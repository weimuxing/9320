
#include "System\All.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG
#ifdef _DEBUG
#define msg(a)            gm_Print(a,0)
#define msg_i(a,b)         gm_Print(a,b)
#else
#define msg(a)
#define msg_i(a,b)
#endif

#if (FEATURE_COLORWARP == REV1)
// Default color corner coordinates:
ROM BYTE DefaultColor[6][3] =
{
   // R G B
   {1, 0, 0}, // R
   {0, 1, 0}, // G
   {0, 0, 1}, // B
   {0, 1, 1}, // C
   {1, 1, 0}, // Y
   {1, 0, 1}  // M
};

// Default Hue Matrix:
ROM SBYTE DefaultHueMatrix[6][3] =
{
   // R G B
   {0, 1, -1}, // R
   {-1, 0, 1}, // G
   {1, -1, 0}, // B
   {0, -1, 1}, // C
   {-1, 1, 0}, // Y
   {1, 0, -1}  // M
};


// ITU601 // x10000
ROM WORD LumaTransfer601[3]=
{
   // 0.2990, 0.5870, 0.1140
   0xBAE, 0x16EE, 0x474
};

// ITU709 // x10000
ROM WORD LumaTransfer709[3] =
{
   // 0.2126, 0.7152, 0.0722
   0x84E, 0x1BF0, 0x2D2
};

#define LumaTransferScale 10000 // 32768

#else // (FEATURE_COLORWARP == REV2)
// MatYCC = [0.3 0.59 0.11 ; -0.1686 -.331 .5 ; .5 -.419 -.081];
ROM SWORD MatYCC[9] =
#if 1
{
    307,  604, 113, //  0.3000,  0.590,  0.110
   -173, -339, 512, // -0.1686, -0.331,  0.500
    512, -429, -83, //  0.5000, -0.419, -0.081
};
#else
{
    306,  601, 117, //  0.299,  0.587,  0.114
   -176, -347, 523, // -0.172, -0.339,  0.511
    523, -483, -85, //  0.511, -0.428, -0.083
};
#endif

ROM SWORD MatYCCinv[9] =
#if 1
{
   1024,    7, 1436, //     1,  0.007,  1.402,
   1024, -344, -730, //     1, -0.336, -0.713,
   1024, 1823,    1, // 0.999,  1.780,  0.001
};
#else
{
   1024,    0, 1405, // 1,      0,  1.372,
   1024, -345, -715, // 1, -0.337, -0.698,
   1024, 1775,   -2, // 1,  1.734, -0.002
};
#endif
#endif // (FEATURE_COLORWARP == REV1)



#define ColorWrapScale 1024 // 1024
// Hue -1 ~ +1 default 0 ==>  -50 ~ + 50
#define UserHueScale 50
// Saturation 0 ~ 2 default 1 for 6-axis, 0 for Black and White ==> 0 ~ 100
#define UserSatScale 50 // 100
// Luma -1 ~ +1 default 0 ==> -50 ~ +50
#define UserLumaScale 50

#define ColorWrapRegisterMask 0x3FFF

#define SRGB_EN 				BIT0
#define SRGB_BLACK_OFFSET_EN 	BIT1
#define SRGB_WSAT_EN 			BIT2|BIT3 // 00 : OFF 01: WPSAT with indepdent RGB components 10: WPSAT whthin white vertex only 11: Default OFF
#define SRGB_GAMUTCLIP_EN 		BIT4 // 0: OFF, 1: ON


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************



//******************************************************************************
//  C O D E
//******************************************************************************
#if (FEATURE_COLORWARP == REV1)
void AdjustSixAxisColor(BYTE AxisId)
{
   SWORD LumaNew, LumaRef, NewBrightness, FinalBrightness;
   SWORD SW_ColorDelta[3];
   SWORD SW_CalTemp[3];
   WORD NewColor[3];
   SWORD SW_Result[3];
   SWORD SW_AdjHue;
   WORD W_AdjSat;
   SWORD W_AdjLuma;

   if (AxisId == RED_AXIS)
   {
      W_AdjSat = UserPrefAcm3RedSat;
      W_AdjLuma = UserPrefAcm3RedLuma;
      SW_AdjHue = UserPrefAcm3RedHue;
   }
   else if (AxisId == GREEN_AXIS)
   {
      W_AdjSat = UserPrefAcm3GreenSat;
      W_AdjLuma = UserPrefAcm3GreenLuma;
      SW_AdjHue = UserPrefAcm3GreenHue;
   }
   else if (AxisId == BLUE_AXIS)
   {
      W_AdjSat = UserPrefAcm3BlueSat;
      W_AdjLuma = UserPrefAcm3BlueLuma;
      SW_AdjHue = UserPrefAcm3BlueHue;
   }
   else if (AxisId == CYAN_AXIS)
   {
      W_AdjSat = UserPrefAcm3CyanSat;
      W_AdjLuma = UserPrefAcm3CyanLuma;
      SW_AdjHue = UserPrefAcm3CyanHue;
   }
   else if (AxisId == YELLOW_AXIS)
   {
      W_AdjSat = UserPrefAcm3YellowSat;
      W_AdjLuma = UserPrefAcm3YellowLuma;
      SW_AdjHue = UserPrefAcm3YellowHue;
   }
   else if (AxisId == MAGENTA_AXIS)
   {
      W_AdjSat = UserPrefAcm3MagentaSat;
      W_AdjLuma = UserPrefAcm3MagentaLuma;
      SW_AdjHue = UserPrefAcm3MagentaHue;
   }
   else
   {
      gm_Printf("Error Axis Id", 0);
   }

   W_AdjSat = (WORD)((DWORD)W_AdjSat * ColorWrapScale / UserSatScale);
   W_AdjLuma = (SWORD)((SDWORD)W_AdjLuma * ColorWrapScale / UserLumaScale);
   SW_AdjHue = (SWORD)((SDWORD)SW_AdjHue * ColorWrapScale / UserHueScale);

   {
      BYTE i;

      LumaNew = 0;
      LumaRef = 0;

      for (i = 0; i < 3; i++)
      {
         SWORD SW_Temp = SW_AdjHue * DefaultHueMatrix[AxisId][i];
         WORD W_TempLumaT;

         if (AxisId < 3)
            SW_ColorDelta[i] = (SW_Temp > 0) ? SW_Temp:0;
         else
            SW_ColorDelta[i] = (SW_Temp < 0) ? SW_Temp:0;

         //gm_Printf("SW_ColorDelta[%d] %d", i, SW_ColorDelta[i]);

         SW_CalTemp[i] = SW_ColorDelta[i] + DefaultColor[AxisId][i] * ColorWrapScale;

         //gm_Printf("SW_CalTemp[%d] = %d", i, SW_CalTemp[i]);

         SW_ColorDelta[i] = (SWORD)((SDWORD)SW_CalTemp[i] * W_AdjSat / ColorWrapScale);

         //gm_Printf("SW_ColorDelta[%d] %d", i, SW_ColorDelta[i]);

         if (gm_IsSDVideo(CH_A))
            W_TempLumaT = LumaTransfer601[i];
         else
            W_TempLumaT = LumaTransfer709[i];

         LumaNew = LumaNew + (WORD)((SDWORD)SW_ColorDelta[i] * W_TempLumaT / LumaTransferScale);
         LumaRef = LumaRef + (WORD)((SDWORD)SW_CalTemp[i] * W_TempLumaT / LumaTransferScale);

         //gm_Printf("LumaNew %d", LumaNew);
         //gm_Printf("LumaRef %d", LumaRef);


      }

      NewBrightness = LumaRef - LumaNew;
      //gm_Printf("NewBrightness %d \n", NewBrightness);

      FinalBrightness = W_AdjLuma + NewBrightness;
      //gm_Printf("FinalBrightness %d \n", FinalBrightness);

      for (i = 0; i < 3; i++)
      {
         NewColor[i] =  SW_ColorDelta[i] + FinalBrightness;

         SW_Result[i] = (SWORD)NewColor[i] - DefaultColor[AxisId][i] * ColorWrapScale;

         //gm_Printf("SW_Result[%d] = 0x%x", i, SW_Result[i]);
      }
      //gm_Printf("\n", 0);

   }

   if ((gm_ReadRegWord(SRGB_CTRL)&SRGB_EN) != SRGB_EN)
   {  
		gm_SetRegBitsWord(SRGB_CTRL, BIT13|BIT14|BIT15|SRGB_EN); // recommand by Jim to open clock early     
   }

   // Program Result to Register ...
   if (AxisId == RED_AXIS)
   {
      gm_WriteRegWord(SRGB_RED_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_RED_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_RED_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else if (AxisId == GREEN_AXIS)
   {
      gm_WriteRegWord(SRGB_GREEN_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_GREEN_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_GREEN_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else if (AxisId == BLUE_AXIS)
   {
      gm_WriteRegWord(SRGB_BLUE_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_BLUE_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_BLUE_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else if (AxisId == CYAN_AXIS)
   {
      gm_WriteRegWord(SRGB_CYAN_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_CYAN_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_CYAN_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else if (AxisId == YELLOW_AXIS)
   {
      gm_WriteRegWord(SRGB_YELLOW_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_YELLOW_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_YELLOW_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else if (AxisId == MAGENTA_AXIS)
   {
      gm_WriteRegWord(SRGB_MAGENTA_R, SW_Result[0]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_MAGENTA_G, SW_Result[1]&ColorWrapRegisterMask);
      gm_WriteRegWord(SRGB_MAGENTA_B, SW_Result[2]&ColorWrapRegisterMask);
   }
   else
   {
      gm_Printf("Error Six Axis Programing", 0);
   }

   if(IsColorWarpApply() == FALSE)
      AdjustColorSpace();
   else
   	gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
}

#else // #if (FEATURE_COLORWARP == REV2)

void AdjustColorWarp(BYTE AxisId)
{
   #define ColorWrapScale_6 1020 // need to be 6x
   SDWORD W_AdjSat;
   SDWORD W_AdjLuma, SW_AdjHue;   
   SDWORD W_r, W_g, W_b;
   SDWORD DW_r, DW_g, DW_b;

	if(AxisId == RED_AXIS)
	{
		W_AdjSat = UserPrefAcm3RedSat;
		W_AdjLuma = UserPrefAcm3RedLuma;
		SW_AdjHue = UserPrefAcm3RedHue;	
	}
	else if(AxisId == GREEN_AXIS)
	{
		W_AdjSat = UserPrefAcm3GreenSat;
		W_AdjLuma = UserPrefAcm3GreenLuma;
		SW_AdjHue = UserPrefAcm3GreenHue;	
	}
	else if(AxisId == BLUE_AXIS)
	{
		W_AdjSat = UserPrefAcm3BlueSat;
		W_AdjLuma = UserPrefAcm3BlueLuma;
		SW_AdjHue = UserPrefAcm3BlueHue;	
	}
	else if(AxisId == CYAN_AXIS)
	{
		W_AdjSat = UserPrefAcm3CyanSat;
		W_AdjLuma = UserPrefAcm3CyanLuma;
		SW_AdjHue = UserPrefAcm3CyanHue;	
	}
	else if(AxisId == YELLOW_AXIS)
	{
		W_AdjSat = UserPrefAcm3YellowSat;
		W_AdjLuma = UserPrefAcm3YellowLuma;
		SW_AdjHue = UserPrefAcm3YellowHue;	
	}
	else if(AxisId == MAGENTA_AXIS)
	{
		W_AdjSat = UserPrefAcm3MagentaSat;
		W_AdjLuma = UserPrefAcm3MagentaLuma;
		SW_AdjHue = UserPrefAcm3MagentaHue;	
	}
	else if(AxisId == BLACK_AXIS)
	{
		W_AdjSat = UserPrefAcm3BlackSat;
		W_AdjLuma = UserPrefAcm3BlackLuma;
		SW_AdjHue = UserPrefAcm3BlackHue;	
	}   
	else if(AxisId == WHITE_AXIS)
	{
		W_AdjSat = UserPrefAcm3WhiteSat;
		W_AdjLuma = UserPrefAcm3WhiteLuma;
		SW_AdjHue = UserPrefAcm3WhiteHue;	
	}   
	else
	{
		gm_Printf("Error Axis Id", 0);
	}

   msg_i("W_AdjSat 0x%X", W_AdjSat);
   msg_i("W_AdjLuma 0x%X", W_AdjLuma);
   msg_i("SW_AdjHue 0x%X", SW_AdjHue);

   W_AdjSat = (WORD)((DWORD)W_AdjSat * ColorWrapScale / UserSatScale);
   W_AdjLuma = (SWORD)((SDWORD)W_AdjLuma * ColorWrapScale / UserLumaScale);
   SW_AdjHue = (SWORD)((SDWORD)SW_AdjHue * ColorWrapScale_6 / UserHueScale);

   msg_i("W_AdjSat 0x%X", W_AdjSat);
   msg_i("W_AdjLuma 0x%X", W_AdjLuma);
   msg_i("SW_AdjHue 0x%X", SW_AdjHue);

   // The hue circle is divided into 6 equal 60 degree arcs.            
   if(AxisId == RED_AXIS)
   {
      // red_hue = mod(1/6*HSBin(1,1)+ 0 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+6*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == GREEN_AXIS)
   {
      // grn_hue = mod(1/6*HSBin(1,2) + 2/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+8*ColorWrapScale_6)/6)%ColorWrapScale_6;               
   }
   else if(AxisId == BLUE_AXIS)
   {
      // blu_hue = mod(1/6*HSBin(1,3) + 4/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+10*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == CYAN_AXIS)
   {
      // cy_hue  = mod(1/6*HSBin(1,4) + 3/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+9*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == YELLOW_AXIS)
   {
      // ye_hue  = mod(1/6*HSBin(1,6) + 1/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+7*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == MAGENTA_AXIS)
   {
      // mg_hue  = mod(1/6*HSBin(1,5) + 5/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+11*ColorWrapScale_6)/6)%ColorWrapScale_6;	
   }   
   else // BLACK_AXIS & WHITE_AXIS
   {
      // black_hue = mod(1/2*HSBin(1,7) + 0 + 4/6, 1);
   	SW_AdjHue = ((3*SW_AdjHue+4*ColorWrapScale_6)/6)%ColorWrapScale_6;	
   }

   // h = 6.*hin;
   SW_AdjHue *= 6;

   {            
      SDWORD B_k, B_k0, B_k1, B_k2, B_k3, B_k4, B_k5;      
      SDWORD W_p, W_s, W_t,W_n, W_v, W_f;

      W_s = ColorWrapScale_6; // Fixed input for current use.
      W_v = ColorWrapScale_6; // Fixed input for current use.

      // k = floor(h);
      B_k = SW_AdjHue/ColorWrapScale_6;

      B_k0 = B_k1 = B_k2 = B_k3 = B_k4 = B_k5 = 0;

      if(B_k == 0)
         B_k0 = 1;
      else if(B_k == 1)
         B_k1 = 1;
      else if(B_k == 2)
         B_k2 = 1;
      else if(B_k == 3)
         B_k3 = 1;
      else if(B_k == 4)
         B_k4 = 1;      
      else
         B_k5 = 1;      

      // p = h-k;
      W_p = SW_AdjHue - B_k*ColorWrapScale_6;
      // t = 1-s;
      W_t = ColorWrapScale_6 - W_s;
      // n = 1-s.*p;
      W_n = (WORD)(ColorWrapScale_6 - (DWORD)W_s * W_p / ColorWrapScale_6);
      // p = 1-(s.*(1-p));
      W_p = (WORD)(ColorWrapScale_6 - ((DWORD)W_s * (ColorWrapScale_6 - W_p) / ColorWrapScale_6));

      // r = k0    + k1.*n + k2.*t + k3.*t + k4.*p + k5;
      W_r = B_k0 * ColorWrapScale_6 + B_k1 * W_n + B_k2 * W_t + B_k3 * W_t + B_k4 * W_p + B_k5 * ColorWrapScale_6;
      // g = k0.*p + k1    + k2    + k3.*n + k4.*t + k5.*t;
      W_g = B_k0 * W_p + B_k1 * ColorWrapScale_6 + B_k2 * ColorWrapScale_6 + B_k3 * W_n + B_k4 * W_t + B_k5 * W_t;
      // b = k0.*t + k1.*t + k2.*p + k3    + k4    + k5.*n;
      W_b = B_k0 * W_t + B_k1 * W_t + B_k2 * W_p + B_k3 * ColorWrapScale_6 + B_k4 * ColorWrapScale_6 + B_k5 * W_n;

      // f = v./max([max(r(:)); max(g(:)); max(b(:))]);
      W_f = (WORD)(((DWORD)W_v * ColorWrapScale_6) / max(W_r, max(W_g, W_b)));
      // r = f.*r;
      W_r = (WORD)((DWORD)W_f * W_r / ColorWrapScale_6);
      // g = f.*g;
      W_g = (WORD)((DWORD)W_f * W_g / ColorWrapScale_6);
      // b = f.*b;
      W_b = (WORD)((DWORD)W_f * W_b / ColorWrapScale_6);

      // mapping to 10-bit
      W_r = (WORD)((DWORD)W_r * ColorWrapScale / ColorWrapScale_6);
      W_g = (WORD)((DWORD)W_g * ColorWrapScale / ColorWrapScale_6);
      W_b = (WORD)((DWORD)W_b * ColorWrapScale / ColorWrapScale_6);

      msg_i("W_r = 0x%X", W_r);
      msg_i("W_g = 0x%X", W_g);
      msg_i("W_b = 0x%X", W_b);
      
   }         

   if((AxisId != BLACK_AXIS) && (AxisId != WHITE_AXIS))
   {      
      SDWORD DW_y, DW_cb, DW_cr;

      // red_ycc = MatYCC * red_rgb';
      DW_y = ((SDWORD)MatYCC[0] * W_r + (SDWORD)MatYCC[1] * W_g + (SDWORD)MatYCC[2] * W_b) / ColorWrapScale;
      DW_cb = ((SDWORD)MatYCC[3] * W_r + (SDWORD)MatYCC[4] * W_g + (SDWORD)MatYCC[5] * W_b) / ColorWrapScale;
      DW_cr = ((SDWORD)MatYCC[6] * W_r + (SDWORD)MatYCC[7] * W_g + (SDWORD)MatYCC[8] * W_b) / ColorWrapScale;

      msg_i("DW_y 0x%X", DW_y);
      msg_i("DW_cb 0x%X", DW_cb);
      msg_i("DW_cr 0x%X", DW_cr);

      // red_yccs = red_ycc.*[1+red_bri red_sat red_sat]';
      DW_y = DW_y * (ColorWrapScale + W_AdjLuma) / ColorWrapScale;
      DW_cb = DW_cb * W_AdjSat / ColorWrapScale;
      DW_cr = DW_cr * W_AdjSat / ColorWrapScale;

      msg_i("1 DW_y 0x%X", DW_y);
      msg_i("1 DW_cb 0x%X", DW_cb);
      msg_i("1 DW_cr 0x%X", DW_cr);      

      // red_new = MatYCCinv*red_yccs;
      DW_r = ((SDWORD)MatYCCinv[0] * DW_y + (SDWORD)MatYCCinv[1] * DW_cb + (SDWORD)MatYCCinv[2] * DW_cr) / ColorWrapScale;
      DW_g = ((SDWORD)MatYCCinv[3] * DW_y + (SDWORD)MatYCCinv[4] * DW_cb + (SDWORD)MatYCCinv[5] * DW_cr) / ColorWrapScale;
      DW_b = ((SDWORD)MatYCCinv[6] * DW_y + (SDWORD)MatYCCinv[7] * DW_cb + (SDWORD)MatYCCinv[8] * DW_cr) / ColorWrapScale;      

      msg_i("DW_r 0x%X", DW_r);      
      msg_i("DW_g 0x%X", DW_g);
      msg_i("DW_b 0x%X", DW_b);
      
   }
   else if(AxisId == BLACK_AXIS)
   {
      DW_r = (DWORD)W_r * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_g = (DWORD)W_g * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_b = (DWORD)W_b * W_AdjSat / ColorWrapScale + W_AdjLuma;
   }
   else if(AxisId == WHITE_AXIS)
   {
      DW_r = (DWORD)W_r * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_g = (DWORD)W_g * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_b = (DWORD)W_b * W_AdjSat / ColorWrapScale + W_AdjLuma;
   }   

	if((gm_ReadRegWord(SRGB_CTRL)&SRGB_EN) != SRGB_EN)
   {  
		gm_SetRegBitsWord(SRGB_CTRL, BIT13|BIT14|BIT15|SRGB_EN|SRGB_BLACK_OFFSET_EN); // BIT13|BIT14|BIT15 recommand by Jim to open clock early
   }
	
	// Program Result to Register ...
	if(AxisId == RED_AXIS)
	{	   
	   DW_r -= ColorWrapScale;
		gm_WriteRegWord(SRGB_RED_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_RED_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_RED_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == GREEN_AXIS)
	{
	   DW_g -= ColorWrapScale;
		gm_WriteRegWord(SRGB_GREEN_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_GREEN_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_GREEN_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == BLUE_AXIS)
	{
	   DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_BLUE_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLUE_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLUE_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == CYAN_AXIS)
	{
	   DW_g -= ColorWrapScale;
      DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_CYAN_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_CYAN_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_CYAN_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == YELLOW_AXIS)
	{
	   DW_r -= ColorWrapScale;
      DW_g -= ColorWrapScale;
		gm_WriteRegWord(SRGB_YELLOW_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_YELLOW_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_YELLOW_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == MAGENTA_AXIS)
	{
	   DW_r -= ColorWrapScale;
      DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_MAGENTA_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_MAGENTA_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_MAGENTA_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == BLACK_AXIS)
	{
		gm_WriteRegWord(SRGB_BLACK_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLACK_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLACK_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == WHITE_AXIS)
	{
		gm_WriteRegWord(SRGB_WHITE_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_WHITE_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_WHITE_B, DW_b&ColorWrapRegisterMask);
	}   
	else
	{
		gm_Printf("Error Six Axis Programing", 0);
	}		

	if(gm_IsValidSignal(gmvb_MainChannel))
	{
	#if 0 //Neil 140712- Already done in AdjustPresetModes();//120915 disable from merge standard code will casue system boot up take more time
   	if(IsColorWarpApply() == FALSE)
   	{   
      	AdjustColorSpace();
   	}
   	else
	#endif
		gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);  
	}
}

#endif // #if (FEATURE_COLORWARP == REV1)

BOOL IsColorWarpApply(void)
{
   if(gm_ReadRegWord(SRGB_CTRL)&SRGB_EN)
   {
      WORD W_Value = 0;
      WORD W_RegIndex = SRGB_BLACK_R;

      while(W_RegIndex < SRGB_GUARD_DIST)
      {
         W_Value = gm_ReadRegWord(W_RegIndex);
          
         if((W_Value != 0) && (W_Value != 0x3FFF))
         {
            if((stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_601) ||
               (stInput[gmvb_MainChannel].Flags & gmd_INPUT_xvYCC_709))
            {
               CLRBIT(stInput[gmvb_MainChannel].Flags, gmd_INPUT_xvYCC_601|gmd_INPUT_xvYCC_709);
               if(UserPrefColorSpace == ColorSpace_XvYcc)
               {
                  UserPrefColorSpace = ColorSpace_sRGB;
                  AdjustColorSpace();
               }
            }         
            
            // Force ODP_3x3 to be unit to make color warp in good operation range.
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF11, 0x400);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF12, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF13, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF21, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF22, 0x400);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF23, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF31, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF32, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_MULT_COEF33, 0x400);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET1, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET2, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_IN_OFFSET3, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET1, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET2, 0);
            gm_WriteRegWord(GAMMA_LUT_MATRIX_OUT_OFFSET3, 0);         
            
            // Set reserved bit when SRGB is applied ...
            gm_SetRegBitsWord(SRGB_CTRL, BIT13|BIT14|BIT15);
            return TRUE;
         }
         
         W_RegIndex += 2;
      }
   }
   
   // Clear reserved bit when SRGB is not applied ...
   gm_ClearRegBitsWord(SRGB_CTRL, BIT13|BIT14|BIT15);
   
   return FALSE;
}


#ifdef DELL_U2410F//120523 Edward for SixAxis color restore default by scheme mode
void AdjustColorWarpDefault(BYTE AxisId)
{
   #define ColorWrapScale_6 1020 // need to be 6x
   SDWORD W_AdjSat;
   SDWORD W_AdjLuma, SW_AdjHue;   
   SDWORD W_r, W_g, W_b;
   SDWORD DW_r, DW_g, DW_b;

	if((AxisId == BLACK_AXIS) || (AxisId == WHITE_AXIS))
		W_AdjSat = 0;
	else		
		W_AdjSat = WB_6AXIS_SAT_DEFAULT;

	W_AdjLuma = WB_6AXIS_LUMA_DEFAULT;
	SW_AdjHue = WB_6AXIS_HUE_DEFAULT;	

  
   W_AdjSat = (WORD)((DWORD)W_AdjSat * ColorWrapScale / UserSatScale);
   W_AdjLuma = (SWORD)((SDWORD)W_AdjLuma * ColorWrapScale / UserLumaScale);
   SW_AdjHue = (SWORD)((SDWORD)SW_AdjHue * ColorWrapScale_6 / UserHueScale);

   // The hue circle is divided into 6 equal 60 degree arcs.            
   if(AxisId == RED_AXIS)
   {
      // red_hue = mod(1/6*HSBin(1,1)+ 0 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+6*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == GREEN_AXIS)
   {
      // grn_hue = mod(1/6*HSBin(1,2) + 2/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+8*ColorWrapScale_6)/6)%ColorWrapScale_6;               
   }
   else if(AxisId == BLUE_AXIS)
   {
      // blu_hue = mod(1/6*HSBin(1,3) + 4/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+10*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == CYAN_AXIS)
   {
      // cy_hue  = mod(1/6*HSBin(1,4) + 3/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+9*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == YELLOW_AXIS)
   {
      // ye_hue  = mod(1/6*HSBin(1,6) + 1/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+7*ColorWrapScale_6)/6)%ColorWrapScale_6;
   }
   else if(AxisId == MAGENTA_AXIS)
   {
      // mg_hue  = mod(1/6*HSBin(1,5) + 5/6 + 1, 1);
   	SW_AdjHue = ((SW_AdjHue+11*ColorWrapScale_6)/6)%ColorWrapScale_6;	
   }   
   else // BLACK_AXIS & WHITE_AXIS
   {
      // black_hue = mod(1/2*HSBin(1,7) + 0 + 4/6, 1);
   	SW_AdjHue = ((3*SW_AdjHue+4*ColorWrapScale_6)/6)%ColorWrapScale_6;	
   }

   // h = 6.*hin;
   SW_AdjHue *= 6;

   {            
      SDWORD B_k, B_k0, B_k1, B_k2, B_k3, B_k4, B_k5;      
      SDWORD W_p, W_s, W_t,W_n, W_v, W_f;

      W_s = ColorWrapScale_6; // Fixed input for current use.
      W_v = ColorWrapScale_6; // Fixed input for current use.

      // k = floor(h);
      B_k = SW_AdjHue/ColorWrapScale_6;

      B_k0 = B_k1 = B_k2 = B_k3 = B_k4 = B_k5 = 0;

      if(B_k == 0)
         B_k0 = 1;
      else if(B_k == 1)
         B_k1 = 1;
      else if(B_k == 2)
         B_k2 = 1;
      else if(B_k == 3)
         B_k3 = 1;
      else if(B_k == 4)
         B_k4 = 1;      
      else
         B_k5 = 1;      

      // p = h-k;
      W_p = SW_AdjHue - B_k*ColorWrapScale_6;
      // t = 1-s;
      W_t = ColorWrapScale_6 - W_s;
      // n = 1-s.*p;
      W_n = (WORD)(ColorWrapScale_6 - (DWORD)W_s * W_p / ColorWrapScale_6);
      // p = 1-(s.*(1-p));
      W_p = (WORD)(ColorWrapScale_6 - ((DWORD)W_s * (ColorWrapScale_6 - W_p) / ColorWrapScale_6));

      // r = k0    + k1.*n + k2.*t + k3.*t + k4.*p + k5;
      W_r = B_k0 * ColorWrapScale_6 + B_k1 * W_n + B_k2 * W_t + B_k3 * W_t + B_k4 * W_p + B_k5 * ColorWrapScale_6;
      // g = k0.*p + k1    + k2    + k3.*n + k4.*t + k5.*t;
      W_g = B_k0 * W_p + B_k1 * ColorWrapScale_6 + B_k2 * ColorWrapScale_6 + B_k3 * W_n + B_k4 * W_t + B_k5 * W_t;
      // b = k0.*t + k1.*t + k2.*p + k3    + k4    + k5.*n;
      W_b = B_k0 * W_t + B_k1 * W_t + B_k2 * W_p + B_k3 * ColorWrapScale_6 + B_k4 * ColorWrapScale_6 + B_k5 * W_n;

      // f = v./max([max(r(:)); max(g(:)); max(b(:))]);
      W_f = (WORD)(((DWORD)W_v * ColorWrapScale_6) / max(W_r, max(W_g, W_b)));
      // r = f.*r;
      W_r = (WORD)((DWORD)W_f * W_r / ColorWrapScale_6);
      // g = f.*g;
      W_g = (WORD)((DWORD)W_f * W_g / ColorWrapScale_6);
      // b = f.*b;
      W_b = (WORD)((DWORD)W_f * W_b / ColorWrapScale_6);

      // mapping to 10-bit
      W_r = (WORD)((DWORD)W_r * ColorWrapScale / ColorWrapScale_6);
      W_g = (WORD)((DWORD)W_g * ColorWrapScale / ColorWrapScale_6);
      W_b = (WORD)((DWORD)W_b * ColorWrapScale / ColorWrapScale_6);
      
   }         

   if((AxisId != BLACK_AXIS) && (AxisId != WHITE_AXIS))
   {      
      SDWORD DW_y, DW_cb, DW_cr;

      // red_ycc = MatYCC * red_rgb';
      DW_y = ((SDWORD)MatYCC[0] * W_r + (SDWORD)MatYCC[1] * W_g + (SDWORD)MatYCC[2] * W_b) / ColorWrapScale;
      DW_cb = ((SDWORD)MatYCC[3] * W_r + (SDWORD)MatYCC[4] * W_g + (SDWORD)MatYCC[5] * W_b) / ColorWrapScale;
      DW_cr = ((SDWORD)MatYCC[6] * W_r + (SDWORD)MatYCC[7] * W_g + (SDWORD)MatYCC[8] * W_b) / ColorWrapScale;

      // red_yccs = red_ycc.*[1+red_bri red_sat red_sat]';
      DW_y = DW_y * (ColorWrapScale + W_AdjLuma) / ColorWrapScale;
      DW_cb = DW_cb * W_AdjSat / ColorWrapScale;
      DW_cr = DW_cr * W_AdjSat / ColorWrapScale;

      // red_new = MatYCCinv*red_yccs;
      DW_r = ((SDWORD)MatYCCinv[0] * DW_y + (SDWORD)MatYCCinv[1] * DW_cb + (SDWORD)MatYCCinv[2] * DW_cr) / ColorWrapScale;
      DW_g = ((SDWORD)MatYCCinv[3] * DW_y + (SDWORD)MatYCCinv[4] * DW_cb + (SDWORD)MatYCCinv[5] * DW_cr) / ColorWrapScale;
      DW_b = ((SDWORD)MatYCCinv[6] * DW_y + (SDWORD)MatYCCinv[7] * DW_cb + (SDWORD)MatYCCinv[8] * DW_cr) / ColorWrapScale;      

   }
   else if(AxisId == BLACK_AXIS)
   {
      DW_r = (DWORD)W_r * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_g = (DWORD)W_g * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_b = (DWORD)W_b * W_AdjSat / ColorWrapScale + W_AdjLuma;
   }
   else if(AxisId == WHITE_AXIS)
   {
      DW_r = (DWORD)W_r * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_g = (DWORD)W_g * W_AdjSat / ColorWrapScale + W_AdjLuma;
      DW_b = (DWORD)W_b * W_AdjSat / ColorWrapScale + W_AdjLuma;
   }   

	if((gm_ReadRegWord(SRGB_CTRL)&SRGB_EN) != SRGB_EN)
		gm_SetRegBitsWord(SRGB_CTRL, SRGB_EN|SRGB_BLACK_OFFSET_EN);
	
	// Program Result to Register ...
	if(AxisId == RED_AXIS)
	{	   
	   DW_r -= ColorWrapScale;
		gm_WriteRegWord(SRGB_RED_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_RED_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_RED_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == GREEN_AXIS)
	{
	   DW_g -= ColorWrapScale;
		gm_WriteRegWord(SRGB_GREEN_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_GREEN_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_GREEN_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == BLUE_AXIS)
	{
	   DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_BLUE_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLUE_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLUE_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == CYAN_AXIS)
	{
	   DW_g -= ColorWrapScale;
      DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_CYAN_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_CYAN_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_CYAN_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == YELLOW_AXIS)
	{
	   DW_r -= ColorWrapScale;
      DW_g -= ColorWrapScale;
		gm_WriteRegWord(SRGB_YELLOW_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_YELLOW_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_YELLOW_B, DW_b&ColorWrapRegisterMask);	
	}
	else if(AxisId == MAGENTA_AXIS)
	{
	   DW_r -= ColorWrapScale;
      DW_b -= ColorWrapScale;
		gm_WriteRegWord(SRGB_MAGENTA_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_MAGENTA_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_MAGENTA_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == BLACK_AXIS)
	{
		gm_WriteRegWord(SRGB_BLACK_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLACK_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_BLACK_B, DW_b&ColorWrapRegisterMask);
	}
	else if(AxisId == WHITE_AXIS)
	{
		gm_WriteRegWord(SRGB_WHITE_R, DW_r&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_WHITE_G, DW_g&ColorWrapRegisterMask);
		gm_WriteRegWord(SRGB_WHITE_B, DW_b&ColorWrapRegisterMask);
	}   
	else
	{
		gm_Printf("Error Six Axis Programing", 0);
	}		

	gm_SyncUpdate(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
   
}
#endif
