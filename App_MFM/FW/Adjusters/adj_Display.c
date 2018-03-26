/*
    $Workfile:   adj_Display.c  $
    $Revision: 1.62 $
    $Date: 2013/05/24 06:46:52 $
*/
#define __ADJ_DISPLAY_C__

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
// MODULE:      adj_display.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "mem.h"
//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern    PortSchemeDependentUserPrefType ROM SchemeFactoryDefaultInROM[NUM_IMAGE_SCHEMES]; // PSD

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(string,value)         gm_Print(string,value)
#else
#define msg(string,value)
#endif

static BOOL IsChangeToHighlightBorderAllowed(gmt_PHY_CH B_Channel);
static BOOL Bt_IsDDDSSourceChannelChanged = TRUE;
static BYTE SysInModulus, SysOutModulus;
WORD ROM CT_7500_Gain[3]=
{
   0xED,
   0xEE,
   0x100,
};

WORD ROM CT_9300_Gain[3]=
{
   0xD7,
   0xDD,
   0x100,
};

//*****************************************************************************
// G L O B A L S
//*****************************************************************************
// Freeze Global
BOOL B_MainCcFreezedFlag = FALSE;
BOOL B_PipCcFreezedFlag = FALSE;
//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION       : void CscUpdateColors (gmt_PHY_CH B_Channel)
//
// USAGE          : Wrapper for gm_CscUpdateColors function to cast PortDependent
//                            structures from WB to gmt_ColorStruct structure.
//                            Updates the color settings on the selected channel.
//
// INPUT          : B_Channel    -    CH_A or CH_B
//
// OUTPUT         : OK            -    Success
//                  ERR_PARAM    -    Unknown color space detected in specified
//                  channel
//
// GLOBALS        :    PortDependentUserPreferences (R)
//                            PortDependent_PIPUserPreferences (R)
//
//******************************************************************************

/****************************************************************************/
/*	G L O B A L    D E F I N I T I O N S									*/
/****************************************************************************/

//******************************************************************************
// S T A T I C   F U N C T I O N S
//******************************************************************************

//*****************************************************************************
// FUNCTION     : BOOL SetBrightnessPWM
// USAGE        : Brightness adjuster function using PWM
// INPUT        : SBYTE Brightness
//                The UserPrefBrigntness range from -108 ~ 0 ~ 107
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
BOOL SetBrightnessPWM(SBYTE Brightness)
{
#if((DEFAULT_PANEL == LG_WUXGA_LM240WU4_SLB1) || (DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1))
   #define BACKLIGHT_MAX 0x90 // 0xFF
   #define BACKLIGHT_MIN 0x2B
   WORD W_PLUSE;

   W_PLUSE = ((WORD)(Brightness+108)*(BACKLIGHT_MAX-BACKLIGHT_MIN)/(108+107)+BACKLIGHT_MIN) << 8;
    #if defined(CMI_73XX_BOARD)
	{
		#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
		gm_WriteRegWord(BACKLIGHT_B_PWM_CONFIG, PWM0_VSRESET|PWM0_TCLK_DIV|PWM0_CLKSEL);
		gm_WriteRegWord(BACKLIGHT_G_PWM_CONFIG, PWM1_VSRESET|PWM1_TCLK_DIV|PWM1_CLKSEL);
		gm_WriteRegWord(BACKLIGHT_DCR_PWM_CONFIG, PWM2_VSRESET|PWM2_TCLK_DIV|PWM2_CLKSEL);
		#else
		gm_WriteRegWord(BACKLIGHT_B_PWM_CONFIG, PWM0_VSRESET|PWM0_TCLK_DIV|PWM0_CLKSEL|PWM0_GPIOn_EN);
		gm_WriteRegWord(BACKLIGHT_G_PWM_CONFIG, PWM1_VSRESET|PWM1_TCLK_DIV|PWM1_CLKSEL|PWM1_GPIOn_EN);
		gm_WriteRegWord(BACKLIGHT_DCR_PWM_CONFIG, PWM2_VSRESET|PWM2_TCLK_DIV|PWM2_CLKSEL|PWM2_GPIOn_EN);
		#endif
		gm_WriteRegWord(BACKLIGHT_B_PWM_CTRL, W_PLUSE|0x90);
		gm_WriteRegWord(BACKLIGHT_G_PWM_CTRL, W_PLUSE|0x90);
		gm_WriteRegWord(BACKLIGHT_DCR_PWM_CTRL, 0xFFFF); //DCR
	}
	#else
	{
		#if(BACKLIGHT_CONTROL_GPIO == ENABLE)
	    gm_WriteRegWord(PWM3_CONFIG, PWM3_VSRESET|PWM3_TCLK_DIV|PWM3_CLKSEL);
		#else
		gm_WriteRegWord(PWM3_CONFIG, PWM3_VSRESET|PWM3_TCLK_DIV|PWM3_CLKSEL|PWM3_GPIOn_EN);
		#endif
	    gm_WriteRegWord(PWM3_CTRL, W_PLUSE|0x90);
	}
    #endif
   return TRUE;
#else
   UNUSED_VAR(Brightness);
   return FALSE;
#endif   
}

#ifdef DELL_U2410F
//zuguang 20090320
//for component (including HDMI.YPbPr) -> Movie
#define UserPrefColorT12000_R  205
#define UserPrefColorT12000_G  204
#define UserPrefColorT12000_B  253

//zuguang 20090318
//for component (including HDMI.YPbPr) -> Game Video, Nature
#define UserPrefColorT6100_R  253
#define UserPrefColorT6100_G  224
#define UserPrefColorT6100_B  242

#define UserPrefGrColorT6100_R  240 //KevinL 20090504 updated from Vincent
#define UserPrefGrColorT6100_G  240
#define UserPrefGrColorT6100_B  243
#endif

void CscUpdateColors (gmt_PHY_CH Channel)
{
    gmt_ColorStruct st_ca;
	
#ifdef DELL_U2410F	// Modified by ST_hansPENG 090211
    //gmt_GammaDescr ROM * ROM *ptr_GammaDescr = GetGammaTable();

		BYTE B_ImageScheme;
		BYTE B_ColorTemp;
		WORD W_Contrast;
		WORD W_Saturation;
		SWORD sW_Hue;

		   
		   gm_WaitDisplayBlanking_CH(Channel);//121025 Edward for image garbage when scheme change 
		//Neil120727+ add this for preset mode is color space, COLOR_MATRIX_CTRL not be setup
		//#if (NON_SCALING_OUTPUT == 1)
		{
		// Enable OVP Matrix
			if(UserPrefVwdFlag == VWD_DEMO_OFF)//120806 Edward for adjust color do not affect VWD window
			{
				gm_WriteRegDWord_LE(COLOR_MATRIX_CTRL, MAIN_MATRIX_EN|PIP_MATRIX_EN|MAIN_MATRIX_WIN_CTRL);
			}	
			else
			{
				gm_WriteRegDWord_LE(COLOR_MATRIX_CTRL, (MAIN_MATRIX_EN|PIP_MATRIX_EN|(2 << MAIN_MATRIX_WIN_CTRL_SHIFT)));
			}
		}
		//#endif
		   
		if (Channel == gmvb_MainChannel)
		{
			//B_ImageScheme = UserPrefImageScheme;
			B_ImageScheme = UserPrefGraphicsImageMode;
			B_ColorTemp = UserPrefPresetColorTemp;
			//B_ColorTemp = UserPrefColorTemp;

			W_Contrast = UserPrefContrast;
			if (IsInputColorFormat_YUV() && (UserPrefVideoImageMode == Video_NATURE))//KevinL 20090304
				W_Saturation = UserPrefSaturation_YPbPr; 
			else
				W_Saturation = UserPrefSaturation;		
			
			sW_Hue = UserPrefHue;	
			//sW_Hue = (SWORD)((UserPrefHue * 360)/(gm_GetAdjusterMax(ADJV_Hue)- gm_GetAdjusterMin(ADJV_Hue)));	//Neil120720*
		}
		else
		{
			B_ImageScheme = UserPrefPipImageScheme;
			B_ColorTemp = UserPrefPipColorTemp;
	
			W_Contrast = UserPrefPipContrast;
			W_Saturation = UserPrefPipSaturation;
			sW_Hue = UserPrefPipHue;

			//sW_Hue = (SWORD)((UserPrefPipHue * 360)/(gm_GetAdjusterMax(ADJV_PipHue)- gm_GetAdjusterMin(ADJV_PipHue)));	//Neil120720*
		}

#if 0//def DELL_U2410F
	if (((!IS_ENERGY_SMART_ON() && !IS_DCR_ON()) ||(UserPrefIsEnergySmartToggledFlag == TRUE)) && (Channel == gmvb_MainChannel)) //KevinL 20101117
#else	//20120828 Add condition "!IS_ENERGY_SMART_ON()" by KevinChen.
	if (!IS_ENERGY_SMART_ON() && !IS_DCR_ON() && (Channel == gmvb_MainChannel) && (UserPrefBrightnessItemGreyout == 0))
#endif		
		{
			//if((B_SplashComplete == gmd_TRUE))
			AdjustBackLight();
		}
	
		if ((Channel == gmvb_PipChannel) || IsBrightnessAdjustBacklight())
			st_ca.sB_Brightness = ConvertBrightness(SchemeFactoryDefaultInROM[B_ImageScheme].Brightness); 
		else		
			st_ca.sB_Brightness = ConvertBrightness(UserPrefBrightness); 
		
		st_ca.W_Saturation = ConvertSaturation(W_Saturation);
		st_ca.W_Contrast = ConvertContrast(W_Contrast, Channel);	
	
		st_ca.sB_Brightness = 0;//120528 Edward set all mode for default value
		
		if(IsPresetMode_GRAPHICS_CUSTOM_COLOR)
		{			
			st_ca.W_RedGain = ConvertRGBGain(UserPrefUserRedGain);
			st_ca.W_GreenGain = ConvertRGBGain(UserPrefUserGreenGain);
			st_ca.W_BlueGain = ConvertRGBGain(UserPrefUserBlueGain);		

			//120718 Edward correct structure and extend range to 1000 ~ -1000 for more obvious
			st_ca.sB_RedOff = (SWORD)(UserPrefUserRedOff*USER_RGB_OFFSET_SHFIT);
			st_ca.sB_GreenOff = (SWORD)(UserPrefUserGreenOff*USER_RGB_OFFSET_SHFIT);
			st_ca.sB_BlueOff = (SWORD)(UserPrefUserBlueOff*USER_RGB_OFFSET_SHFIT);

			//gm_Print("###st_ca.sB_Brightness %d",st_ca.sB_Brightness);
			//gm_Print("###st_ca.W_Saturation 0x%x",st_ca.W_Saturation);			
		}
		else
		{
			st_ca.sB_RedOff = 0x0;
			st_ca.sB_GreenOff = 0x0;
			st_ca.sB_BlueOff = 0x0;
			
			if (!IsPresetMode_GRAPHICS_COLOR_TEMP)
			{
				if(IsPresetMode_GRAPHICS_MOVIE)//120601 Edward add movie 9300K (Vincent)
				{
					st_ca.W_RedGain = UserPrefColorT9300_R;
					st_ca.W_GreenGain = UserPrefColorT9300_G;
					st_ca.W_BlueGain = UserPrefColorT9300_B;
				}	
				else if(IsPresetMode_GRAPHICS_MULTIMEDIA)//120604 Edward add Multimedia 7200K (DELL)
				{
					st_ca.W_RedGain = UserPrefColorT7200_R;
					st_ca.W_GreenGain = UserPrefColorT7200_G;
					st_ca.W_BlueGain = UserPrefColorT7200_B;
				}	
				else if(IsColorSpace_GRAPHICS_SRGB)
				{
					st_ca.W_RedGain = UserPrefsRGBColor_R;
					st_ca.W_GreenGain = UserPrefsRGBColor_G;
					st_ca.W_BlueGain = UserPrefsRGBColor_B;
				}
				else if(IsColorSpace_GRAPHICS_ADOBE_RGB)
				{
					st_ca.W_RedGain = UserPrefAdobeRGBColor_R;
					st_ca.W_GreenGain = UserPrefAdobeRGBColor_G;
					st_ca.W_BlueGain = UserPrefAdobeRGBColor_B;
				}
				else if(IsColorSpace_GRAPHICS_CAL1 || IsColorSpace_GRAPHICS_CAL2)//120814 Edward add for DELL request
				{
					st_ca.W_RedGain = ConvertRGBGain(USER_GAIN_DEF);
					st_ca.W_GreenGain = ConvertRGBGain(USER_GAIN_DEF);
					st_ca.W_BlueGain = ConvertRGBGain(USER_GAIN_DEF);	
				}
				else
				{
				 	st_ca.W_RedGain = UserPrefColorT6500_R;
					st_ca.W_GreenGain = UserPrefColorT6500_G;
					st_ca.W_BlueGain = UserPrefColorT6500_B;
				}	
			}
			else//for Preset mode Color temp mode
			{
				switch (B_ColorTemp)
				{
			case CTEMP_5000K:	// Graphics_WARM, Video_MOVIE	//5000K
				st_ca.W_RedGain = UserPrefColorT5000_R;
				st_ca.W_GreenGain = UserPrefColorT5000_G;
				st_ca.W_BlueGain = UserPrefColorT5000_B;				
				break;			
	
			case CTEMP_5700K:	// YPbPr_GameVideo, YPbPr_Nature  //zuguang 20090318				
				st_ca.W_RedGain = UserPrefColorT5700_R;
				st_ca.W_GreenGain = UserPrefColorT5700_G;
				st_ca.W_BlueGain = UserPrefColorT5700_B;		
				break;
				
		    	case CTEMP_6500K:	// Graphics_STANDARD
				st_ca.W_RedGain = UserPrefColorT6500_R;
				st_ca.W_GreenGain = UserPrefColorT6500_G;
				st_ca.W_BlueGain = UserPrefColorT6500_B;
				break;		

			case CTEMP_7200K:	// 120604 Edward for DELL request for Multimedia
				st_ca.W_RedGain = UserPrefColorT7200_R;
				st_ca.W_GreenGain = UserPrefColorT7200_G;
				st_ca.W_BlueGain = UserPrefColorT7200_B;
				break;	
				
			case CTEMP_7500K:	// Graphics_STANDARD
				st_ca.W_RedGain = UserPrefColorT7500_R;
				st_ca.W_GreenGain = UserPrefColorT7500_G;
				st_ca.W_BlueGain = UserPrefColorT7500_B;
				break;					
				
			case CTEMP_9300K:	// Graphics_COOL
				st_ca.W_RedGain = UserPrefColorT9300_R;
				st_ca.W_GreenGain = UserPrefColorT9300_G;
				st_ca.W_BlueGain = UserPrefColorT9300_B;						
				break;			
	
			case CTEMP_10000K:	// YPbPr Movie				
				st_ca.W_RedGain = UserPrefColorT10000_R;
				st_ca.W_GreenGain = UserPrefColorT10000_G;
				st_ca.W_BlueGain = UserPrefColorT10000_B;
				break;
				
			case CTEMP_USER:	//User color Temp			
				st_ca.W_RedGain = ConvertRGBGain(UserPrefUserRedGain);
				st_ca.W_GreenGain = ConvertRGBGain(UserPrefUserGreenGain);
				st_ca.W_BlueGain = ConvertRGBGain(UserPrefUserBlueGain);
				break;		
			default:
				//default settings - unity
				st_ca.W_RedGain = 0x100;
				st_ca.W_GreenGain = 0x100;
				st_ca.W_BlueGain = 0x100;
				st_ca.sB_RedOff = 0x0;
				st_ca.sB_GreenOff = 0x0;
				st_ca.sB_BlueOff = 0x0;
				break;
				}
			}
	  }	


		st_ca.sW_Hue = ConvertHue(sW_Hue);	
		
		if(st_ca.sW_Hue > 0)//120801 Jp for DELL hue range extend
			   st_ca.sW_Hue = sW_Hue*455/50; // 455 is tested by customer with U2413


		//Hyb140731* Pip Channel follow Main "Standard"
		if(Channel == gmvb_PipChannel)
		{
			//120921 Edward PIP channel color temp follow Main "standard"
			st_ca.W_RedGain = UserPrefColorT6500_R;
			st_ca.W_GreenGain = UserPrefColorT6500_G;
			st_ca.W_BlueGain = UserPrefColorT6500_B;
			
			st_ca.sB_RedOff = 0x0;
			st_ca.sB_GreenOff = 0x0;
			st_ca.sB_BlueOff = 0x0;
			st_ca.sW_Hue = 0x0;			
		}			   
#else

    st_ca.sB_Brightness = UserPrefBrightness;
    st_ca.W_Saturation = UserPrefSaturation;
    st_ca.W_Contrast = UserPrefContrast;

    #ifdef BRIGHTNESS_PWM
   	if (SetBrightnessPWM(UserPrefBrightness))
      	st_ca.sB_Brightness = 0;
	#endif

   switch(UserPrefColorTemp)
   {
      case CTEMP_USER:
         {
            st_ca.W_RedGain = UserPrefUserRedGain;
            st_ca.W_GreenGain = UserPrefUserGreenGain;
            st_ca.W_BlueGain = UserPrefUserBlueGain;
         }
         break;

      case CTEMP_7500K:
         {
            st_ca.W_RedGain = CT_7500_Gain[0];
            st_ca.W_GreenGain = CT_7500_Gain[1];
            st_ca.W_BlueGain = CT_7500_Gain[2];
         }
         break;
      
      case CTEMP_9300K:
         {            
            st_ca.W_RedGain = CT_9300_Gain[0];
            st_ca.W_GreenGain = CT_9300_Gain[1];
            st_ca.W_BlueGain = CT_9300_Gain[2];         
         }
         break;

      //case CTEMP_6500K:
      default:
         {            
   st_ca.W_RedGain = 0x100;
   st_ca.W_GreenGain = 0x100;
   st_ca.W_BlueGain = 0x100;
         }         
         break;
   }

   if(UserPrefColorTemp == CTEMP_USER)
   {
      st_ca.sB_RedOff = UserPrefUserRedOff;
      st_ca.sB_GreenOff = UserPrefUserGreenOff;
      st_ca.sB_BlueOff = UserPrefUserBlueOff;      
   }
   else
   {
   st_ca.sB_RedOff = 0x0;
   st_ca.sB_GreenOff = 0x0;
   st_ca.sB_BlueOff = 0x0;
   }
	

   // if input is PAL, should not change the hue (ie. default value should be used)
   st_ca.sW_Hue = (IsInputPal()) ? (SchemeFactoryDefaultInROM[UserPrefImageScheme].Hue) : (UserPrefHue);
#endif//end of #ifdef DELL_U2410F

    #ifdef YUV_GAIN_ADJ_IN_YUV_DOMAIN   
        st_ca.W_YGain = UserPrefUserY; 
        st_ca.W_UGain = UserPrefUserU;
        st_ca.W_VGain = UserPrefUserV; 
    #endif    

#ifdef Contrast_0_Black	//yvonne 20120614
	//YUV domain only
	//20120906 yvonne add PresetMode-MultiMedia/Movie/Game at RGB mode contrast to 0 not black
    if((IsInputColor_YPbPr())||((!IsInputColor_YPbPr())&&(IsPresetMode_GRAPHICS_MOVIE ||IsPresetMode_GRAPHICS_MULTIMEDIA||IsPresetMode_GRAPHICS_GAME)) || (gm_ReadRegWord(ACM3_CTRL) & ACM3_EN)) //KevinL 20120920 updated for VWD enabled
	//if((IsInputColor_YPbPr())||((!IsInputColor_YPbPr())&&(IsPresetMode_GRAPHICS_MOVIE ||IsPresetMode_GRAPHICS_MULTIMEDIA||IsPresetMode_GRAPHICS_GAME)))
	{
		BYTE i;
		if(st_ca.W_Contrast <50)
		{
		    st_ca.W_Saturation = 0;
			i= st_ca.W_Contrast / 5;
			st_ca.W_Saturation = st_ca.W_Saturation+ 20*i ;
		}
	}
	
	if (Channel == gmvb_PipChannel)//20120820 Add by KevinChen for HDMI in Pip channel contrast set to 0 not black screen.(current input RGB will trans to YUV)
	{	//gm_Print("gmvb_PipChannel - %d",gmvb_PipChannel);
		//gm_Print("UserPrefPipColorFormat - %d",UserPrefPipColorFormat);
		//if(IsPipInputColorFormat_YUV())
		{//gm_Print("Pip YUV",0);
			if(IsPIPHDMIInput())
			{	
				BYTE i;
				//gm_Print("A st_ca.W_Saturation - %d",st_ca.W_Saturation);
				if(st_ca.W_Contrast <50)
				{
					st_ca.W_Saturation = 0;
					i= st_ca.W_Contrast / 5;
					st_ca.W_Saturation = st_ca.W_Saturation+ 20*i ;
				}
									//gm_Print("B st_ca.W_Saturation - %d",st_ca.W_Saturation);
			}
		}
	}
#endif
	//120628 Edward for SixAxis color restore default by scheme mode	
	{// ColorWarp
      BYTE i;

      #if (FEATURE_COLORWARP == REV1)
      for(i=0; i<6; i++)
      #else
      for(i=0; i<8; i++)
      #endif
      {
         #if (FEATURE_COLORWARP == REV1)
        	 AdjustSixAxisColor(i);
         #else // (FEATURE_COLORWARP == REV2)		 
		 	if(IsPresetMode_GRAPHICS_CUSTOM_COLOR)
         		AdjustColorWarp(i);
			else
				AdjustColorWarpDefault(i);
         #endif // (FEATURE_COLORWARP == REV1)
      }
   }

	// OVP 3x3 matrix update
    gm_CscUpdateColors(Channel, &st_ca);
	
	#if 1//defined(CMI_73XX_BOARD)//120717 Edward add test from Vincent request in SDC // 120727 KimChye enable for all
	if((IsPresetMode_GRAPHICS_TEXT)&&(!(gm_ReadRegWord(ip_control[gmvb_MainChannel])&IMP_YUV_EN)))//Neil120727+ check if YUV domain or not, for RGB interlace mode, color abnormal
	{
		gm_WriteRegWord(IMP_IN_OFFSET1, 0x1010);
    	gm_WriteRegWord(IMP_IN_OFFSET2, 0x10A0);
    	gm_WriteRegWord(IMP_IN_OFFSET3, 0x1010);

		gm_WriteRegWord(IMP_OUT_OFFSET1, 0x7F0);
    	gm_WriteRegWord(IMP_OUT_OFFSET2, 0x760);
    	gm_WriteRegWord(IMP_OUT_OFFSET3, 0x7F0);

		gm_SetRegBitsWord(IMP_CONTROL, IMP_CSC_OVERRIDE); //120718 KimChye
	}
	else
	{
		gm_WriteRegWord(IMP_IN_OFFSET1, 0);
    	gm_WriteRegWord(IMP_IN_OFFSET2, 0);
    	gm_WriteRegWord(IMP_IN_OFFSET3, 0);

		gm_WriteRegWord(IMP_OUT_OFFSET1, 0);
    	gm_WriteRegWord(IMP_OUT_OFFSET2, 0);
    	gm_WriteRegWord(IMP_OUT_OFFSET3, 0);

		gm_ClearRegBitsWord(IMP_CONTROL,IMP_CSC_OVERRIDE);  //120718 KimChye
	}	
	#endif
	
}

//******************************************************************************
//
// FUNCTION       : void SetupColorSpace(gmt_PHY_CH B_Channel)
//
// USAGE          : Setup color space of the selected channel signal type
//
// INPUT          : B_Channel    = gmvb_MainChannel (CH_A) or
//                     gmvb_PipChannel  (CH_B)
//
// OUTPUT         : None
//
// GLOBALS        : ModeIndependentUserPreferences.ColorControlGroup    (R)
//
// USED_REGS      : MAIN_MULT_CTRL    (W)
//                  PIP_MULT_CTRL    (W)
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetupColorSpace(gmt_PHY_CH B_Channel)
{
   gmt_PHY_CH B_CscChannel;

#if (FEATURE_CROSS_POINT_SWITCH == ENABLE)

   // if TRUE, MAIN channel uses PIP 3x3 and vice versa
#ifdef UserPrefColorResourceRouting
   if (UserPrefColorResourceRouting == 1)
   {
      B_CscChannel = gm_OtherChannel(B_Channel);
   }
   else
      B_CscChannel = B_Channel;
#endif
#else
   B_CscChannel = B_Channel;
#endif

	gm_WaitDisplayBlanking_CH(B_Channel);//121025 Edward for image garbage when scheme change 
//#if (NON_SCALING_OUTPUT == 1)
//#else
	{
	     // Enable OVP Matrix     
		if(UserPrefVwdFlag == VWD_DEMO_OFF)//120806 Edward for adjust color do not affect VWD window
		{
			gm_WriteRegDWord_LE(COLOR_MATRIX_CTRL, MAIN_MATRIX_EN|PIP_MATRIX_EN|MAIN_MATRIX_WIN_CTRL);
		}	
		else
		{
			gm_WriteRegDWord_LE(COLOR_MATRIX_CTRL, MAIN_MATRIX_EN|PIP_MATRIX_EN|(2 << MAIN_MATRIX_WIN_CTRL_SHIFT));
		}
	}
//#endif

   /*
   if(UserPrefImageScheme)
   {
      if(!(gm_ReadRegWord(ACM3_CTRL) & ACM3_EN))
      {
         gm_VideoProcessingEnable (VIDEO_ACC);
         gm_VideoProcessingEnable (VIDEO_ACM);
      }

   }
   */

   //
   // Update Set B coefficients for Split Screen Demo mode
   // NOTE: This has to be performed after SET A coefficients are programmed
   //
   if (B_CscChannel == gmvb_MainChannel)
   {
   #if (FEATURE_VWD == ENABLE)
      if (IsVwdDetected() == TRUE)
      {
         gm_VideoProcessingAdjustment(VIDEO_ACM, ACC_ACM_INSIDE_HIGHLIGHT_WIN);
      }
      else
   #endif
      {
#if defined(UserPrefSplitScreen)
         if (UserPrefSplitScreen)
         {    // Setup video window and program Set B of coefficients+offsets
            gm_CscSetVideoWindow(DEMO_WINDOW_SPLIT);
         }
         else
         {
            gm_CscSetVideoWindow(VIDEO_WIN_DISABLED);
         }
         AdjustAccAcmSplit();
#endif
      }
   }

   #if 0
   // RGB2YUV
   if ((gm_ReadRegWord (IMP_CONTROL)  & IMP_YUV_EN)
         || (((gm_ReadRegWord(ACM3_CTRL) & ACM3_EN) == 0)   // No ACM and ACC enabled
            && ((gm_ReadRegWord(ACC_CTRL) & ACC_ENABLE) == 0)))
   {
      gm_ClearRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);      
      gm_Printf("~~~~ YUV input or ACC ACM are not enable ... %d", B_Channel);
   }
   else
   {
      gm_SetRegBitsWord(OVP_CTRL, RGB2YUV_WIN_CTRL);
      gm_Printf("~~~~ RGB input and ACC ACM are enable ... %d", B_Channel);
   }    
   #endif

    //
    // 3x3 Colour Matrix
    //
    CscUpdateColors(B_Channel);
   
}

//******************************************************************************
//
// FUNCTION       : void Freeze(gmt_PHY_CH B_Channel, BOOL B_State)
//
// USAGE          : Freeze the input picture on selected channel by
//                  disabling the input capture
//
// INPUT          : B_Channel    = gmvb_MainChannel (CH_A) or
//                     gmvb_PipChannel  (CH_B)
//                  B_State     = TRUE or FALSE
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
void Freeze(gmt_PHY_CH B_Channel, BOOL B_State)
{
   //
   // Main
   //
   if (B_Channel == gmvb_MainChannel)
   {
      if (B_State == TRUE)
      {
		 if(gm_IsModeComplete(B_Channel))
         		gm_InputWaitVsync(gmvb_MainChannel,1);
		 gm_InputCaptureFreeze(gmvb_MainChannel, B_State);
      }
      else
      {
		  if(DisplayFreeze == gmd_FALSE)
		  gm_InputCaptureFreeze(gmvb_MainChannel, B_State);
      }

   }
   //
   // PIP
   //
   else
   {
      if (B_State == TRUE)
      {
		  if(gm_IsModeComplete(B_Channel))
         		gm_InputWaitVsync(gmvb_PipChannel,1);
		  gm_InputCaptureFreeze(gmvb_PipChannel, B_State);
      }
      else
      {
		  if(DisplayFreeze == gmd_FALSE)
		  gm_InputCaptureFreeze(gmvb_PipChannel, B_State);
      }
   }
}

//******************************************************************************
//
// FUNCTION       : static BOOL IsChangeToHighlightBorderAllowed(gmt_PHY_CH B_Channel)
//
// USAGE          : Return if highlight border is allowed in single-pip mode. This
//                        function returns TRUE when
//                     1) the highlight border is used in place of pip border (this is TRUE when
//                        the channels are mapped such that gmvb_MainChannel=CH_B and
//                        gmvb_PipChannel = CH_A; and
//                     2) the specified B_Channel is used as the PIP window on the screen in
//                        single-pip mode.
//
// INPUT          : B_Channel    = gmvb_MainChannel (CH_A) or
//                     gmvb_PipChannel  (CH_B)
//
// OUTPUT         : BOOL
//
// GLOBALS        : gmvb_MainChannel, gmvb_PipChannel
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BOOL IsChangeToHighlightBorderAllowed(gmt_PHY_CH B_Channel)
{
   return (IsSinglePipMode(UserPrefPipMode)&&(gmvb_MainChannel!=CH_A)
           &&(B_Channel==gmvb_PipChannel));
}

#ifndef NO_PIP_SUPPORT
//******************************************************************************
//
// FUNCTION       : void SetPipBorder(gmt_PHY_CH B_Channel, BOOL Bt_State)
//
// USAGE          : Set pip border or highlight border depending on the pip mode. In
//                      the case of channel mapping swapped or output windows swapped,
//                      use highlight border in place of pip border. This substitution is allowed
//                      only in single-pip mode.
//
// INPUT          : B_Channel    = gmvb_MainChannel (CH_A) or
//                     gmvb_PipChannel  (CH_B)
//                  B_State        = TRUE or FALSE
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_OutputHStart
//                    gmvw_OutputVStart
//                    gmvw_OutputHActive
//                    gmvw_OutputVActive
//                    B_FocusBorder
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void SetPipBorder(gmt_PHY_CH B_Channel, BOOL Bt_State)
{
   if (UserPrefPipMode == NO_PIP)
   {
      gm_PipBorderEnable(FALSE);
      gm_HighlightBorder(FALSE,0,0,0,0);
   }
   else if (IsSinglePipMode(UserPrefPipMode))
   {
#ifdef PIP_BORDER_ENABLED
      //need to use highlight border in place of pip border
      if (gmvb_MainChannel != CH_A)
      {
         //disable pip border since highlight border should be used instead
         gm_PipBorderEnable(FALSE);
		if(PxpBorderFlag == gmd_FALSE)
			gm_PipBorderSetup(0, 0, 0);
		else
			gm_PipBorderSetup(PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT, RGB_565(PIP_BORDER_COLOR));
	
         //allow changes to highlight border only when highlight border
         //  is used in place of pip border and the B_Channel is used
         //  as a PIP window in single-pip mode
         if (IsChangeToHighlightBorderAllowed(B_Channel))
         {
            if (Bt_State)
            {
               gmt_WindowStruct tempWin;

               // No PIP border when MAIN is CH_B; use highlight border instead
               if (gmvw_OutputHStart[B_Channel] > PIP_BORDER_WIDTH)
                  tempWin.W_Hstart = gmvw_OutputHStart[B_Channel] - PIP_BORDER_WIDTH;
               else
                  tempWin.W_Hstart = 0;
               if (gmvw_OutputVStart[B_Channel] > PIP_BORDER_HEIGHT)
                  tempWin.W_Vstart = gmvw_OutputVStart[B_Channel] - PIP_BORDER_HEIGHT;
               else
                  tempWin.W_Vstart = 0;
               tempWin.W_Width = gmvw_OutputHActive[B_Channel] + (2 * PIP_BORDER_WIDTH);
               tempWin.W_Height = gmvw_OutputVActive[B_Channel] + (2 * PIP_BORDER_HEIGHT);
			if(PxpBorderFlag == gmd_TRUE)
	                    gm_HighlightBorder(TRUE, PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT,
	                        RGB_565(PIP_BORDER_COLOR), &tempWin);
			else
                		gm_HighlightBorder(FALSE,0,0,0,0);
				
            }
            else
            {
               gm_HighlightBorder(FALSE,0,0,0,0);
            }
         }
      }
      else//otherwise use pip border in single-pip mode
      {
         gm_HighlightBorder(FALSE,0,0,0,0);

         if (B_Channel == CH_B)//but only CH_B can turn on pip border
         {
            if (Bt_State)
            {
				if(PxpBorderFlag == gmd_TRUE)
            	{
            		gm_PipBorderSetup(PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT, RGB_565(PIP_BORDER_COLOR));
            		gm_PipBorderEnable(TRUE);
				}
				else
                {
					gm_PipBorderSetup(0, 0, 0);
                	gm_PipBorderEnable(FALSE);
				}

            }
            else
               gm_PipBorderEnable(FALSE);
         }
      }
#endif
   }
   else if (IsSideBySidePipMode(UserPrefPipMode))//highlight border only in side-by-side
   {
#ifdef HIGHLIGHT_BORDER_ENABLED
        gm_PipBorderEnable(FALSE);

	if(PxpBorderFlag == gmd_FALSE)
		gm_PipBorderSetup(0, 0, 0);
	else
		gm_PipBorderSetup(PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT, RGB_565(PIP_BORDER_COLOR));
		
		
      //modify highlight border only if B_Channel has the focus
      if (B_FocusBorder == B_Channel)
      {
         //
         // Remove Border
         //
         if (Bt_State == FALSE)
         {
            gm_HighlightBorder(FALSE,0,0,0,0);
         }
         else
         {
            gmt_WindowStruct tempWin;

            // Enable highlight border
            if (gmvw_OutputHStart[B_Channel] > PIP_BORDER_WIDTH)
               tempWin.W_Hstart = gmvw_OutputHStart[B_Channel] - PIP_BORDER_WIDTH;
            else
               tempWin.W_Hstart = 0;
            if (gmvw_OutputVStart[B_Channel] > PIP_BORDER_HEIGHT)
               tempWin.W_Vstart = gmvw_OutputVStart[B_Channel] - PIP_BORDER_HEIGHT;
            else
               tempWin.W_Vstart = 0;
            tempWin.W_Width = gmvw_OutputHActive[B_Channel] + (2 * PIP_BORDER_WIDTH);
            tempWin.W_Height = gmvw_OutputVActive[B_Channel] + (2 * PIP_BORDER_HEIGHT);
			
			if(PxpBorderFlag == gmd_TRUE)
		    {
				gm_PipBorderSetup(PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT, RGB_565(PIP_BORDER_COLOR));
	    		gm_HighlightBorder(TRUE, PIP_BORDER_WIDTH, PIP_BORDER_HEIGHT,
	                    RGB_565(HIGHLIGHT_BORDER_COLOR), &tempWin);
				gm_PipBorderEnable(TRUE);
			}
			else
            {
            	gm_HighlightBorder(FALSE,0,0,0,0);
				gm_PipBorderEnable(FALSE);
			}
				
	        }
        }
#endif
    }
   else //if(IsPxpFlexibleMode(UserPrefPipMode)||IsPxpQuarterMode(UserPrefPipMode)||IsPxpPupMode(UserPrefPipMode))
   {
	    PxpBorderFlag = gmd_FALSE;
		gm_PipBorderEnable(FALSE);
		gm_PipBorderSetup(0, 0, 0);
      	gm_HighlightBorder(FALSE,0,0,0,0);
   }
}
#endif //NO_PIP_SUPPORT

//******************************************************************************
//
// FUNCTION       : void DisplayLockSource(void)
//
// USAGE          : Main()
//                  ModeSetupHdlr()
//                  ModeSetup()
//                  DisplayLockSource()
//                  Application Level routine to determine DDDS Lock Source
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : B_ModeSetupFlag            (R)
//                  gmvb_MainChannel        (R)
//                  gmvb_PipChannel            (R)
//                  gmvb_ChannelInput        (R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void DisplayLockSource(void)
{
    gmt_PHY_CH InitDDDSSourceChannel = gmvb_DDDSSourceChannel;
    
#if (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1) 	//20121022 Kordonm modify for VGA source (pip HDMI(DVD player), pip off), VGA display garbage
	gmvb_DDDSSourceChannel = gmvb_MainChannel;
#else    
    gmvb_DDDSSourceChannel = 
        !gm_IsValidSignal(gmvb_PipChannel) ? gmvb_MainChannel :
        !gm_IsValidSignal(gmvb_MainChannel) ? gmvb_PipChannel :
        gm_IsVideo(gmvb_MainChannel) ? gmvb_MainChannel :
        gm_IsVideo(gmvb_PipChannel) ? gmvb_PipChannel : 
        gmvb_MainChannel;
#endif
   if((UserPrefPipMode == NO_PIP) && (gmvb_DDDSSourceChannel == gmvb_PipChannel))
   {
      gmvb_DDDSSourceChannel = gmvb_MainChannel;
   }
   // Keep track of change in DDDS source and if valid signal is on DDDS source channel
   // Flag Bt_IsDDDSSourceChannelChanged is cleared only by running DisplayClockLock()
   if ((InitDDDSSourceChannel != gmvb_DDDSSourceChannel)
     ||((!gm_IsValidSignal(gmvb_DDDSSourceChannel)) && (B_FreeRun[gmvb_DDDSSourceChannel] == FALSE))
     ||(gm_IsValidSignal(gmvb_DDDSSourceChannel) && B_FreeRun[gmvb_DDDSSourceChannel]))
   {
      Bt_IsDDDSSourceChannelChanged = TRUE;     
   }
}

//******************************************************************************
//******************************************************************************
DWORD GetDisplayRateDefault(void)
{
   if (PanelMinVFreq > gmc_DEF_DISPLAY_RATE)
   {
      return PanelMinVFreq;
   }
   else
   {
      return gmc_DEF_DISPLAY_RATE;
   }
}

//******************************************************************************
// FUNCTION       : void SetDddsRefLineVal(WORD W_SrcHtotal)
// USAGE          : Program display reference line value and compare period settings
// INPUT          : Source HTotal
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : 
// POST_CONDITION :
//******************************************************************************
void SetDddsRefLineVal(WORD W_SrcHtotal)
{
   WORD W_InputVTotal, W_RefLine;
   DWORD D_InputTotal, D_DisplayTotal;
   DWORD D_Numerator, D_Denominator, D_CompareRatio, D_Fraction, D_Remainder, D_Test;
   BYTE B_ScaleDown, B_Shift;   
   
   W_InputVTotal = stInput[gmvb_DDDSSourceChannel].Vtotal;   
   D_InputTotal = (DWORD)W_SrcHtotal * W_InputVTotal;        
   if (gm_IsInterlaced(gmvb_DDDSSourceChannel))
   {
      W_InputVTotal = (W_InputVTotal & ~1) * 2 + 1;
      //SCR4035 re-calculate D_InputTotal
      D_InputTotal = (DWORD)W_SrcHtotal * W_InputVTotal;
      D_InputTotal /= 2;
   }

   D_DisplayTotal = (DWORD)gmv_OUTPUT_HTOTAL * gmv_OUTPUT_VTOTAL;

   msg("gmvb_DDDSSourceChannel = %d",gmvb_DDDSSourceChannel);
   msg("W_SrcHtotal = %d",W_SrcHtotal);
   msg("stInput[gmvb_DDDSSourceChannel].Vtotal = %d",stInput[gmvb_DDDSSourceChannel].Vtotal);
   msg("stInput[gmvb_DDDSSourceChannel].Vfrq = %d",stInput[gmvb_DDDSSourceChannel].Vfrq);
   msg("W_InputVTotal = %d",W_InputVTotal);
   msg("gmv_OUTPUT_HTOTAL = %d",gmv_OUTPUT_HTOTAL);
   msg("gmv_OUTPUT_VTOTAL = %d",gmv_OUTPUT_VTOTAL);
   msg("D_InputTotal = 0x%lX",D_InputTotal);
   msg("D_DisplayTotal = 0x%lX",D_DisplayTotal);

   D_InputTotal = D_InputTotal * SysInModulus;

   D_DisplayTotal = D_DisplayTotal * SysOutModulus;   

   if(W_SrcHtotal <= 4095)
   	W_RefLine = W_SrcHtotal;
   else
   {
      W_SrcHtotal = 0xFFF;
      W_RefLine = 0xFFF; // Max
   }

   // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
   B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;
   if (B_ScaleDown > 1) // may need to scale down more then 2 ...
   {
      while ((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
      {
         B_ScaleDown++;
         //gm_Printf("~~~~ B_ScaleDown %d", B_ScaleDown);
      }
   }
   //gm_Printf("B_ScaleDown %d", B_ScaleDown);
   D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
   D_Denominator = D_InputTotal / B_ScaleDown;
   D_CompareRatio = D_Numerator / D_Denominator;
   //gm_Printf("D_CompareRatio = 0x%X", D_CompareRatio);

   {
      BYTE B_Ref_ScaleDown = 1;

      //gm_Printf("%d", gm_GetSystemTime());
      while (D_CompareRatio > 4095)    // Use Vertical Ratio
      {
         B_Ref_ScaleDown = (BYTE)((D_CompareRatio/4095) + 1); // Make sure D_CompareRatio < 4095
         W_RefLine = W_SrcHtotal / B_Ref_ScaleDown;            

         // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
         B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;    
         if (B_ScaleDown > 1) // may need to scale down more then 2 ...
         {
            while ((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
            {
               B_ScaleDown++;
               //gm_Printf("~~~~ 1 B_ScaleDown %d", B_ScaleDown);
            }
         }
         //gm_Printf("CASE %d B_ScaleDown %d", B_Ref_ScaleDown, B_ScaleDown);
         D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
         D_Denominator = D_InputTotal / B_ScaleDown;
         D_CompareRatio = D_Numerator / D_Denominator;            
         //gm_Printf("D_CompareRatio %d", D_CompareRatio);
      }   
      //gm_Printf("W_RefLine %d", W_RefLine);
      //gm_Printf("B_Ref_ScaleDown %d", B_Ref_ScaleDown);
      //gm_Printf("%d", gm_GetSystemTime());
   }

   //======================================================
   // Calculate Fractional of ((RLine * DTotal) / ITotal) * 2^24
   //    = Integer   = (int)(Numerator / Denominator)
   //    = Remainder = (Numerator - (Integer * Denominator))
   //    = Fraction  = (Remainder / Denominator) * 2^24
   //    = Fraction  = ((Remainder * 2^(x)) / Denominator) * 2^(24-x)
   D_Remainder = D_Numerator - (D_CompareRatio * D_Denominator);
   B_Shift = 0;
   D_Test = D_Remainder;
   while (((D_Test & 0x80000000L) == 0) && (B_Shift < 24))
   {
      ++B_Shift;
      D_Test = D_Test << 1;
   }        
   D_Fraction = ((D_Remainder << B_Shift) / D_Denominator) << (24 - B_Shift);

   // Program Compare & Ref Line
   gm_WriteRegWord(DDDS1_REF_LINE_VAL, W_RefLine);
   gm_WriteRegWord(DDDS1_COMP_INTEGER, (WORD)(D_CompareRatio & 0xffff));
   gm_WriteRegDWord(DDDS1_COMP_FRACTION, D_Fraction);
}

//******************************************************************************
// FUNCTION       : void gm_DddsLock_M(void)
// USAGE          : Program display clock in certain number of steps to prevent panel flickering
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  : DisplayLockSource() should have been called to update
//                    gmvb_DDDSSourceChannel
// POST_CONDITION :
//******************************************************************************
gmt_RET_STAT gm_DddsLock_M(void)
{
   if (gmvb_DDDSSourceChannel == CH_B)
      gm_SetRegBitsWord(CLOCK_CONFIG2, IP_REF_CLK_SEL);
   else //if (gmvb_DDDSSourceChannel == CH_A)
      gm_ClearRegBitsWord(CLOCK_CONFIG2, IP_REF_CLK_SEL);
   
   SetDddsRefLineVal(stInput[gmvb_DDDSSourceChannel].Htotal);

   gm_DdsForceCloseLoop(CLK_DDDS);
   gm_SetRegBitsWord(DDDS1_INIT, BIT0);
   gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);
   
   // Update Register
   if (gm_IsInterlaced(gmvb_DDDSSourceChannel))
      gm_SetRegBitsWord(SYNC_CONTROL, (INTERLACE_EN|ODD_EN));
   else
      gm_ClearRegBitsWord(SYNC_CONTROL, (INTERLACE_EN|ODD_EN));

 #ifdef ST_4K2K_93xx_BOARD
   if (GetChipID_4K2K() == CHIPID_4K2K_L)
      gm_SetRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
   else
      gm_ClearRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
 #else
   if (gmvb_DDDSSourceChannel == CH_B)
      gm_SetRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
   else
      gm_ClearRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
 #endif

   //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
   gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);

   #if 0 // 1 // Add DDDS checking threshold
   if(gm_DdsWaitStable(CLK_DDDS) == OK)
   {           
      BYTE B_TimerOutCount = 0;

      gm_Printf("~~~~ system time in %d ~~~~", gm_GetSystemTime());
      gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);
      while(gm_ReadRegDWord(DDDS1_CUR_FREQ) == 0)
      {               
         B_TimerOutCount++;
         if(B_TimerOutCount > 15)
         {
            gm_Printf("DDDS Time Out", 0);
            break;
         }
         //gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);
         gm_Delay1ms(1);
      }
      gm_Printf("~~~~ system time out %d ~~~~", gm_GetSystemTime());
      gm_Printf("~~~~ B_TimerOutCount %d", B_TimerOutCount);
      gm_WriteRegWord(AVDDS1_CUR_FREQ, 0xFF);
      gm_Printf("~~~~ AVDDS1_CUR_FREQ 0x%lX ~~~~", gm_ReadRegDWord(AVDDS1_CUR_FREQ));
      gm_Printf("~~~~ DDDS1_CUR_FREQ 0x%lX ~~~~", gm_ReadRegDWord(DDDS1_CUR_FREQ));
      gm_WriteRegWord(DDDS1_FREQUENCY_DELTA_THRESH, (gm_ReadRegDWord(DDDS1_CUR_FREQ) >> 14) / 10);
      return OK;
   }    
   else
   {
      return ERR_TMO;
   }
   #else
   return gm_DdsWaitStable(CLK_DDDS); // Tracking Error
   #endif
}

//******************************************************************************
//******************************************************************************

#if 0
gmt_RET_STAT gm_DddsLock_DFL(WORD B_DesiredOutputFrameRate)
{
   DWORD D_DisplayTotal;
   DWORD D_InputTotal;
   DWORD D_Remainder;
   DWORD D_Numerator;
   DWORD D_Denominator;
   DWORD D_Fraction;
   DWORD D_CompareRatio;
   DWORD D_Test;
   WORD W_RefLine;    
   WORD W_InputVTotal;
   BYTE B_Shift;
   BYTE B_ScaleDown;

   if (gmvb_DDDSSourceChannel == CH_B)
        gm_SetRegBitsWord(CLOCK_CONFIG2, IP_REF_CLK_SEL);
   else //if (gmvb_DDDSSourceChannel == CH_A)
        gm_ClearRegBitsWord(CLOCK_CONFIG2, IP_REF_CLK_SEL);

   //gm_Printf("B_DesiredOutputFrameRate %d", B_DesiredOutputFrameRate);

   W_InputVTotal = stInput[gmvb_DDDSSourceChannel].Vtotal;
   //gm_Printf("stInput[gmvb_DDDSSourceChannel].Vtotal %d Chnannel %d", stInput[gmvb_DDDSSourceChannel].Vtotal, gmvb_DDDSSourceChannel);
   D_InputTotal = (DWORD)stInput[gmvb_DDDSSourceChannel].Htotal * W_InputVTotal;        
   if (gm_IsInterlaced(gmvb_DDDSSourceChannel))
   {
      W_InputVTotal = (W_InputVTotal & ~1) * 2 + 1;
      //SCR4035 re-calculate D_InputTotal
      D_InputTotal = (DWORD)stInput[gmvb_DDDSSourceChannel].Htotal * W_InputVTotal;		 

      D_InputTotal /= 2;
   }

   

   D_DisplayTotal = (DWORD)gmv_OUTPUT_HTOTAL * gmv_OUTPUT_VTOTAL;

   #if 1 // DFL TEST
      #if 1 // debug print
      gm_Delay1ms(1);
      gm_Printf("stInput[gmvb_DDDSSourceChannel].Vfrq %d", stInput[gmvb_DDDSSourceChannel].Vfrq);
      gm_Delay1ms(1);
      gm_Printf("stInput[gmvb_DDDSSourceChannel].Htotal %d", stInput[gmvb_DDDSSourceChannel].Htotal);
      gm_Delay1ms(1);
      gm_Printf("W_InputVTotal %d", W_InputVTotal);   
      gm_Delay1ms(1);
      gm_Printf("gmv_OUTPUT_HTOTAL %d", gmv_OUTPUT_HTOTAL);
      gm_Delay1ms(1);
      gm_Printf("gmv_OUTPUT_VTOTAL %d", gmv_OUTPUT_VTOTAL);
      gm_Delay1ms(1);
      gm_Printf("D_InputTotal 0x%lX", D_InputTotal);
      gm_Delay1ms(1);
      gm_Printf("D_DisplayTotal 0x%lX", D_DisplayTotal);   
      gm_Delay1ms(1);
      #endif

   #if 1            
   if(stInput[gmvb_DDDSSourceChannel].Vfrq)
      D_InputTotal = D_InputTotal * stInput[gmvb_DDDSSourceChannel].Vfrq;
   else
      D_InputTotal = D_InputTotal * B_DesiredOutputFrameRate;
   
   D_DisplayTotal = D_DisplayTotal * B_DesiredOutputFrameRate;   
   #else
   if(stInput[gmvb_DDDSSourceChannel].Vfrq)
      D_InputTotal = D_InputTotal * ((stInput[gmvb_DDDSSourceChannel].Vfrq+5)/10); // +5 for round up ...
   else
      D_InputTotal = D_InputTotal * B_DesiredOutputFrameRate;
   D_DisplayTotal = D_DisplayTotal * B_DesiredOutputFrameRate;
   #endif

   if(stInput[gmvb_DDDSSourceChannel].Vfrq)
   {
      D_InputTotal = D_InputTotal/((B_DesiredOutputFrameRate >= (stInput[gmvb_DDDSSourceChannel].Vfrq))? B_DesiredOutputFrameRate:(stInput[gmvb_DDDSSourceChannel].Vfrq));
      D_DisplayTotal = D_DisplayTotal/((B_DesiredOutputFrameRate >= (stInput[gmvb_DDDSSourceChannel].Vfrq))? B_DesiredOutputFrameRate:(stInput[gmvb_DDDSSourceChannel].Vfrq));
   }
   else
   {
      D_InputTotal = D_InputTotal/B_DesiredOutputFrameRate;
      D_DisplayTotal = D_DisplayTotal/B_DesiredOutputFrameRate;
   }
   #endif
   
   W_RefLine = stInput[gmvb_DDDSSourceChannel].Htotal;

   // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
   B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;
   if(B_ScaleDown > 1) // may need to scale down more then 2 ...
   {
      while((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
      {
         B_ScaleDown++;
         gm_Printf("~~~~ B_ScaleDown %d", B_ScaleDown);
      }
   }
   //gm_Printf("B_ScaleDown %d", B_ScaleDown);
   D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
   D_Denominator = D_InputTotal / B_ScaleDown;
   D_CompareRatio = D_Numerator / D_Denominator;
   //gm_Printf("D_CompareRatio = 0x%X", D_CompareRatio);

   #if 1
   {
      BYTE B_Ref_ScaleDown = 1;

      //gm_Printf("%d", gm_GetSystemTime());
      while (D_CompareRatio > 4095)    // Use Vertical Ratio
      {         
         #if 1
            B_Ref_ScaleDown = (BYTE)((D_CompareRatio/4095) + 1); // Make sure D_CompareRatio < 4095
            W_RefLine = stInput[gmvb_DDDSSourceChannel].Htotal/B_Ref_ScaleDown;            
         #else
            B_Ref_ScaleDown++;
            //gm_Printf("%d CASE %d", gm_GetSystemTime(), B_Ref_ScaleDown);
            #if 1
            W_RefLine -= B_Ref_ScaleDown;
            #else
            W_RefLine = stInput[gmvb_DDDSSourceChannel].Htotal/B_Ref_ScaleDown;
            #endif
         #endif
         // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
         B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;    
         if(B_ScaleDown > 1) // may need to scale down more then 2 ...
         {
            while((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
            {
               B_ScaleDown++;
               gm_Printf("~~~~ 1 B_ScaleDown %d", B_ScaleDown);
            }
         }      
         //gm_Printf("CASE %d B_ScaleDown %d", B_Ref_ScaleDown, B_ScaleDown);
         D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
         D_Denominator = D_InputTotal / B_ScaleDown;
         D_CompareRatio = D_Numerator / D_Denominator;            
         //gm_Printf("D_CompareRatio %d", D_CompareRatio);
      }   
      gm_Printf("W_RefLine %d", W_RefLine);
      gm_Printf("B_Ref_ScaleDown %d", B_Ref_ScaleDown);
      gm_Printf("%d", gm_GetSystemTime());
   }
   #else
   // Ratio (Based on Vertical)
   if (D_CompareRatio > 4095)    // Use Vertical Ratio
   {
      gm_Printf("CASE 1", 0);
      W_RefLine = W_InputVTotal;
      // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
      B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;    
      if(B_ScaleDown > 1)
      {
         while((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
            B_ScaleDown++;
      }      
      gm_Printf("CASE 1 B_ScaleDown %d", B_ScaleDown);
      D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
      D_Denominator = D_InputTotal / B_ScaleDown;
      D_CompareRatio = D_Numerator / D_Denominator;            
   }

   //SCR4035 re-calculate in case D_CompareRatio > 4095, e.g., 480i to WUXGA
   // Ratio (Based on half of Vertical)
   if (D_CompareRatio > 4095)    // Use half of Vertical Ratio
   {
      gm_Printf("CASE 2", 0);
      W_RefLine = W_InputVTotal/2;
      // Check for DWORD overflow situation.  If detected, increment the ScaleDown divider to 2.
      B_ScaleDown = (D_DisplayTotal >= ((DWORD)0xFFFFFFFFL /W_RefLine)) ? 2 : 1;    
      if(B_ScaleDown > 1)
      {
         while((D_DisplayTotal / B_ScaleDown) >= ((DWORD)0xFFFFFFFFL /W_RefLine))
            B_ScaleDown++;
      }      
      gm_Printf("CASE 2 B_ScaleDown %d", B_ScaleDown);
      D_Numerator = (DWORD)W_RefLine * (D_DisplayTotal / B_ScaleDown);
      D_Denominator = D_InputTotal / B_ScaleDown;
      D_CompareRatio = D_Numerator / D_Denominator;            
   }
   #endif
   //======================================================
   // Calculate Fractional of ((RLine * DTotal) / ITotal) * 2^24
   //    = Integer     = (int)(Numerator / Denominator)
   //  = Remainder = (Numerator - (Integer * Denominator))
   //    = Fraction    = (Remainder / Denominator) * 2^24
   //    = Fraction    = ((Remainder * 2^(x)) / Denominator) * 2^(24-x)
   D_Remainder = D_Numerator - (D_CompareRatio * D_Denominator);
   B_Shift = 0;
   D_Test = D_Remainder;
   while (((D_Test & 0x80000000L) == 0) && (B_Shift < 24))
   {
      ++B_Shift;
      D_Test = D_Test << 1;
   }        
   D_Fraction = ((D_Remainder << B_Shift) / D_Denominator) << (24 - B_Shift);

   // Program Compare & Ref Line
   gm_WriteRegWord(DDDS1_REF_LINE_VAL, W_RefLine);
   gm_WriteRegWord(DDDS1_COMP_INTEGER, (WORD)(D_CompareRatio & 0xffff));
   gm_WriteRegDWord(DDDS1_COMP_FRACTION, D_Fraction);

   gm_DdsForceCloseLoop (CLK_DDDS);
   gm_SetRegBitsWord(DDDS1_INIT, BIT0);
   gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);

   // Update Register
   #if 1
   if(gm_IsInterlaced(gmvb_DDDSSourceChannel))
      gm_SetRegBitsWord(SYNC_CONTROL, (INTERLACE_EN|ODD_EN));
   else
      gm_ClearRegBitsWord(SYNC_CONTROL, (INTERLACE_EN|ODD_EN));

   if(gmvb_DDDSSourceChannel == CH_B)
      gm_SetRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
   else
      gm_ClearRegBitsWord(SYNC_CONTROL, LOCK_SOURCE);
   #else
   gm_WriteRegWord(SYNC_CONTROL, 
     ((gmvb_DDDSSourceChannel == CH_B) ? LOCK_SOURCE : 0) |
     (gm_IsInterlaced(gmvb_DDDSSourceChannel) ? (INTERLACE_EN|ODD_EN) : 0));
   #endif
   //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
   gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);

   #if 0 // 1 // Add DDDS checking threshold
   if(gm_DdsWaitStable(CLK_DDDS) == OK)
   {           
      BYTE B_TimerOutCount = 0;

      gm_Printf("~~~~ system time in %d ~~~~", gm_GetSystemTime());
      gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);
      while(gm_ReadRegDWord(DDDS1_CUR_FREQ) == 0)
      {               
         B_TimerOutCount++;
         if(B_TimerOutCount > 15)
         {
            gm_Printf("DDDS Time Out", 0);
            break;
         }
         //gm_WriteRegWord(DDDS1_CUR_FREQ, 0xFF);
         gm_Delay1ms(1);
      }
      gm_Printf("~~~~ system time out %d ~~~~", gm_GetSystemTime());
      gm_Printf("~~~~ B_TimerOutCount %d", B_TimerOutCount);
      gm_WriteRegWord(AVDDS1_CUR_FREQ, 0xFF);
      gm_Printf("~~~~ AVDDS1_CUR_FREQ 0x%lX ~~~~", gm_ReadRegDWord(AVDDS1_CUR_FREQ));
      gm_Printf("~~~~ DDDS1_CUR_FREQ 0x%lX ~~~~", gm_ReadRegDWord(DDDS1_CUR_FREQ));
      gm_WriteRegWord(DDDS1_FREQUENCY_DELTA_THRESH, (gm_ReadRegDWord(DDDS1_CUR_FREQ) >> 14) / 10);
      return OK;
   }    
   else
   {
      return ERR_TMO;
   }
   #else
   return gm_DdsWaitStable(CLK_DDDS); // Tracking Error
   #endif
}
#endif

//******************************************************************************
//******************************************************************************
gmt_RET_STAT FindModulus(DWORD * DW_InModulus, DWORD * DW_OutModulus, WORD B_DesiredOutputFrameRate)
{
   DWORD DW_TempRatio;
   BYTE B_Result = OK;
   
   (*DW_OutModulus) = B_DesiredOutputFrameRate;
   
   if(stInput[gmvb_DDDSSourceChannel].Vfrq)
   {
      (*DW_InModulus) = (stInput[gmvb_DDDSSourceChannel].Vfrq+5)/10;
   }
   else
      (*DW_InModulus) = 60;
   
   DW_TempRatio = ((*DW_InModulus) < (*DW_OutModulus)) ? (*DW_InModulus) : (*DW_OutModulus);

   for(;DW_TempRatio>0;DW_TempRatio--)
   {
      if((((*DW_OutModulus)%DW_TempRatio) == 0) && (((*DW_InModulus)%DW_TempRatio) == 0))
      {
         //gm_Printf("DW_TempRatio %d", DW_TempRatio);
         break;
      }
   }
   (*DW_InModulus) = (*DW_InModulus) / DW_TempRatio;
   (*DW_OutModulus) = (*DW_OutModulus) / DW_TempRatio;

   if((*DW_InModulus) > 31) // Max 31 (SYNC_CONTROL:FRAME_MODULUS)
   {
      gm_Printf("Over DW_InModulus %d", (*DW_InModulus));
      gm_Printf("Over DW_OutModulus %d", (*DW_OutModulus));            
      #if 1
      (*DW_InModulus) = stInput[gmvb_DDDSSourceChannel].Vfrq / 10;
      (*DW_OutModulus) = B_DesiredOutputFrameRate;
      
      DW_TempRatio = ((*DW_InModulus) < (*DW_OutModulus)) ? (*DW_InModulus) : (*DW_OutModulus);

      for(;DW_TempRatio>0;DW_TempRatio--)
      {
         if((((*DW_OutModulus)%DW_TempRatio) == 0) && (((*DW_InModulus)%DW_TempRatio) == 0))
         {
            //gm_Printf("DW_TempRatio %d", DW_TempRatio);
            break;
         }
      }
      (*DW_InModulus) = (*DW_InModulus) / DW_TempRatio;
      (*DW_OutModulus) = (*DW_OutModulus) / DW_TempRatio;               

      if((*DW_InModulus) > 31) // check if can't make by divid by 2 then divid by 5
      {
         gm_Printf("5 DW_InModulus %d", (*DW_InModulus));
         gm_Printf("5 DW_OutModulus %d", (*DW_OutModulus));
         (*DW_InModulus) = stInput[gmvb_DDDSSourceChannel].Vfrq / 10;
         if(((*DW_InModulus) % 5) >= 3) // check closed to whihc side...
         {
            (*DW_InModulus) = ((*DW_InModulus)/5 + 1) * 5;
         }
         else
         {
            (*DW_InModulus) = ((*DW_InModulus)/5) * 5;
         }
         (*DW_OutModulus) = B_DesiredOutputFrameRate;

         DW_TempRatio = ((*DW_InModulus) < (*DW_OutModulus)) ? (*DW_InModulus) : (*DW_OutModulus);

         for(;DW_TempRatio>0;DW_TempRatio--)
         {
            if((((*DW_OutModulus)%DW_TempRatio) == 0) && (((*DW_InModulus)%DW_TempRatio) == 0))
            {
               //gm_Printf("DW_TempRatio %d", DW_TempRatio);
               break;
            }
         }
         (*DW_InModulus) = (*DW_InModulus) / DW_TempRatio;
         (*DW_OutModulus) = (*DW_OutModulus) / DW_TempRatio;                  

         if((*DW_InModulus) > 31)
            gm_Printf("~~~~ Not able to Support even divid by 5 ...", 0);
      }               
      #else
      BYTE B_Divider = 1;
      gm_Printf("~~~~ DW_InModulus is over", DW_InModulus);

      while(DW_InModulus > 31)
      {
         DW_InModulus = (stInput[gmvb_DDDSSourceChannel].Vfrq/10) / (DW_TempRatio*B_Divider);
         B_Divider++;
      }

      gm_Printf("B_Divider = %d", B_Divider);
      if(B_Divider > 1)
         DW_OutModulus = DW_OutModulus / (B_Divider - 1);
      #endif
   }

   
   //gm_Printf("DW_InModulus %d", (*DW_InModulus));
   //gm_Printf("DW_OutModulus %d", (*DW_OutModulus));
   #if 0
   {
      #define ABS_DIFF(a,b)         ((a>b)?(a-b):(b-a))
      SBYTE B_Delta = 0;
      DWORD DW_Diff0 = 0;
      DWORD DW_Diff1 = 0;
      
      DW_Diff0 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * B_DesiredOutputFrameRate * 10);
      DW_Diff1 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * (B_DesiredOutputFrameRate * 10 + 1));

      if(DW_Diff0 == 0)
         B_Delta = 0;
      else if(DW_Diff1 < DW_Diff0)               
         B_Delta = 1;
      else            
         B_Delta = -1;

      gm_Printf("B_Delta = %d", B_Delta);

      while((ABS_DIFF(B_Delta, 0) < 11) && (B_Delta != 0))
      {                                    
         if(B_Delta > 0)
         {                                          
            DW_Diff0 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * (B_DesiredOutputFrameRate * 10 + B_Delta));
            DW_Diff1 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * (B_DesiredOutputFrameRate * 10 + B_Delta + 1));                  
            if(DW_Diff1 > DW_Diff0)
               break;
            else
               B_Delta ++;
         }
         else
         {
            DW_Diff0 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * (B_DesiredOutputFrameRate * 10 + B_Delta));
            DW_Diff1 = ABS_DIFF(DW_OutModulus * stInput[gmvb_DDDSSourceChannel].Vfrq, DW_InModulus * (B_DesiredOutputFrameRate * 10 + B_Delta - 1));                  
            
            if(DW_Diff1 > DW_Diff0)
               break;
            else
               B_Delta --;                     
         }                                  
      }

      gm_Printf("Final B_Delta = %d", B_Delta);
      B_DesiredOutputFrameRate = B_DesiredOutputFrameRate * 10 + B_Delta;              
   }
   #endif
   return B_Result;
}

//******************************************************************************
//******************************************************************************
void DisplayClockLock(gmt_PHY_CH B_Channel, BYTE B_NumOfSteps)
{
   BYTE B_Index;
   SBYTE SB_Delta;
   WORD B_DesiredOutputFrameRate;   
   DWORD DW_InModulus, DW_OutModulus = 0;

   UNUSED_VAR(B_Channel);
   
#if (FEATURE_3D == ENABLE)
   if (gm_3D_Is3DModeEnabled())
   {
      B_DesiredOutputFrameRate = gm_3D_GetDesiredOutputVFreq();
   }
   else
#endif

      if (gm_IsVideo(gmvb_DDDSSourceChannel))
      {
         // PAL: 50 +/- 0.5 Hz
         if ((stInput[gmvb_DDDSSourceChannel].Vfrq >= 495)
               && (stInput[gmvb_DDDSSourceChannel].Vfrq <= 505))
         {
            if (PanelMaxVFreq >= 100)
            {
               B_DesiredOutputFrameRate = 100;
            }
            else
            {
               // if the panel not support 50Hz set to default ...
               if (PanelMinVFreq > 55)
               {
                  B_DesiredOutputFrameRate = (BYTE)GetDisplayRateDefault();
               }
               else
               {
                  B_DesiredOutputFrameRate = 50;
               }
            }
         }
         else
         {
         #if 0 // Athena MC FIFO RD will underflow
            if (PanelMaxVFreq >= 120)
            {
               B_DesiredOutputFrameRate = 120;
            }
            else
         #endif
            {
               B_DesiredOutputFrameRate = 60;
            }
         }
      }
      else
      {
         if ((stInput[gmvb_DDDSSourceChannel].Vfrq/10 >= PanelMinVFreq-1) &&
               (stInput[gmvb_DDDSSourceChannel].Vfrq/10 <= PanelMaxVFreq+1))
         {
            B_DesiredOutputFrameRate = (stInput[gmvb_DDDSSourceChannel].Vfrq+5)/10;
         }
         else
         {
            B_DesiredOutputFrameRate = (BYTE) GetDisplayRateDefault();
         }
      }

   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K() == CHIPID_4K2K_FE) 
   {
      B_DesiredOutputFrameRate = (stInput[gmvb_MainChannel].Vfrq+5)/10;
      B_DisplayLockMethod = DISP_LOCK_NORMAL; // FE force to program lock
      
      if(gm_IsInterlaced(gmvb_MainChannel))
      {      
         B_DesiredOutputFrameRate = B_DesiredOutputFrameRate* 2;
         B_DisplayLockMethod = DISP_FREERUN; // FE Interlace keep Free Run
      }

      #if 0 // 1 // Limit output under 100Hz
      if(B_DesiredOutputFrameRate >= 200)
         B_DesiredOutputFrameRate = B_DesiredOutputFrameRate/4;
      else if(B_DesiredOutputFrameRate >= 100)
         B_DesiredOutputFrameRate = B_DesiredOutputFrameRate/2;      
      #endif
   }
   #endif


   // Return if there is no change in display frequency && no change in DDDS source
   if ((B_DesiredOutputFrameRate == gmvb_OutputFrameRate) && !Bt_IsDDDSSourceChannelChanged)
   {
      return;
   }

   // Set Bt_IsDDDSSourceChannelChanged to FALSE since display clock will be reprogrammed
   Bt_IsDDDSSourceChannelChanged = FALSE;

   if ((B_NumOfSteps == 0)
         || (B_DesiredOutputFrameRate == gmvb_OutputFrameRate))
   {
      B_NumOfSteps = 1;    // minimum number of steps is one
   }

   SB_Delta = ((SBYTE) B_DesiredOutputFrameRate - gmvb_OutputFrameRate) / B_NumOfSteps;

   for (B_Index =1; B_Index <=B_NumOfSteps; B_Index++)
   {
      if (B_Index == B_NumOfSteps)
      {
         gmvb_OutputFrameRate = B_DesiredOutputFrameRate;
      }
      else
      {
         gmvb_OutputFrameRate += SB_Delta;
      }

    #if (ENABLE_DFL == 1)
      gm_ClearRegBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE);
      gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);
    #endif
      gm_DisplayClockSetup();

		FindModulus(&DW_InModulus, &DW_OutModulus, B_DesiredOutputFrameRate);
      SysInModulus = (BYTE)DW_InModulus;
      SysOutModulus = (BYTE)DW_OutModulus;

      if (B_DisplayLockMethod == DISP_LOCK_DFL)
      {
         BYTE B_Result = OK;

         gm_DddsLock_M();

       #ifdef ST_4K2K_93xx_BOARD
         if (GetChipID_4K2K() == CHIPID_4K2K_L) 
         {                  
            gm_ClearAndSetBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE, LOCK_SOURCE|BIT0); // Fixed Lock
            gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);
         }
         else                  
       #endif                              
         {               
            B_Result = gm_DFLLock((BYTE)DW_InModulus, (BYTE)DW_OutModulus);
         }            

         #if 1 // DFL failure check
         if (B_Result != OK)
         {
            gm_ClearRegBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE); // Free Run
            gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);
            gm_DdsForceOpenLoop(CLK_DDDS);
            gm_Printf("~~~~ FREE RUN !!!!", 0);
         }
         #endif
      }
      else if (B_DisplayLockMethod == DISP_LOCK_NORMAL)
      {
         gm_DddsLock_M();
      	gm_WriteRegWord(DH_LOCK_LOAD, 1);
         gm_WriteRegWord(DV_LOCK_LOAD, gm_ReadRegWord(DV_ACTIVE_START) - 1);         
         gm_ClearAndSetBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE, BIT2); // Program Lock         
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K() == CHIPID_4K2K_FE)
         {
            WORD W_SYNC_CTRL_TEMP = 0; //  gm_ReadRegWord(SYNC_CONTROL);
            
            gm_ClearRegBitsWord(SYNC_CONTROL, FRAME_MODULUS);
            W_SYNC_CTRL_TEMP = gm_ReadRegWord(SYNC_CONTROL);
            
            if(SysInModulus == 1)
            {
               W_SYNC_CTRL_TEMP = W_SYNC_CTRL_TEMP|((SysOutModulus - 1) << 4);            
               gm_WriteRegWord(SYNC_CONTROL, W_SYNC_CTRL_TEMP);
         }
         else
               gm_ClearRegBitsWord(SYNC_CONTROL, FRAME_LOCK_MODE);
         }
         #endif
         gm_SetRegBitsWord(HOST_CONTROL, ODP_FORCE_UPDATE);
      }
		else //if (B_DisplayLockMethod == DISP_FREERUN)
      {
         #if 0 // 1 // set DDDS when free run
         gm_DddsLock_M();
         #endif
         gm_DdsForceOpenLoop(CLK_DDDS);
      }
   }
}

//******************************************************************************
// Horizontal Filter window coordinate is 13-bit
// Vertical Filter window coordinate is 12-bit
//******************************************************************************
void SetWindowSharpness(WORD FilterVal, WORD W_HorStart, WORD W_HorEnd, WORD W_VerStart, WORD W_VerEnd, BYTE B_NeedUpdated)
{
   // Set the Demo Sharpness
   gm_WriteRegWord(MHF_DEMOWIND_SHARPNESS, FilterVal);
   gm_WriteRegWord(MVF_DEMOWIND_SHARPNESS, FilterVal);

   // Config Sharpness Demo Window
   if (stInput[gmvb_MainChannel].Hactive <= PanelWidth)
   {
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_2, W_VerStart);
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_3, W_VerEnd);
   }
   else
   {
      DWORD DW_Temp = (DWORD)W_VerStart * stInput[gmvb_MainChannel].Vactive / PanelHeight;
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_2, (WORD)DW_Temp);

      DW_Temp = DW_Temp + (DWORD)(W_VerEnd - W_VerStart) * stInput[gmvb_MainChannel].Vactive / PanelHeight;
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_3, (WORD)DW_Temp);
   }

   gm_WriteRegWord(MVF_DEMOWIND_CTRL_2, W_VerStart);
   gm_WriteRegWord(MVF_DEMOWIND_CTRL_3, W_VerEnd);

   {//This is just a close setting not match one...
      #define NON_SCALE_OFFSET   24 // This value is tested with 1920, 1680 and 1600 panel.
      
      DWORD HScale = gm_GetHScaleValue(gmvb_MainChannel);
      BYTE B_Offset = 0;
      DWORD DW_HTemp;      

      B_Offset = (BYTE)((HScale * NON_SCALE_OFFSET) / 1048576L) + 1; // 2^20 = 1048576

      DW_HTemp = (DWORD)W_HorStart * stInput[gmvb_MainChannel].Hactive / PanelWidth;

      // MVF become a fix value when the input is equal or bigger than panel
      if (stInput[gmvb_MainChannel].Hactive < PanelWidth)
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_0, ((DW_HTemp + 3) << 1) | WINDOW_SHARP_EN);
      else
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_0, ((W_HorStart + 3) << 1) | WINDOW_SHARP_EN);
         
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_0, ((DW_HTemp + B_Offset + 1) << 1) | WINDOW_SHARP_EN);

      DW_HTemp = DW_HTemp + (DWORD)(W_HorEnd - W_HorStart) * stInput[gmvb_MainChannel].Hactive / PanelWidth;
      
      // MVF become a fix value when the input is equal or bigger than panel
      if (stInput[gmvb_MainChannel].Hactive < PanelWidth)
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_1, DW_HTemp + 4);
      else
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_1, W_HorEnd + 4);
         
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_1, DW_HTemp+B_Offset);
   }

   if (B_NeedUpdated)
   {
      gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
   }
}

/***********************************  END  ************************************/
