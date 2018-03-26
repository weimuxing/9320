//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include <stdLib.h>
#include <math.h>
#include "System\All.h"
#include "System\VWD.h"
#include "StdLibInc\gm_ACC3.h"
//#include "stdlibinc\PIXCRMain.h"

//******************************************************************************
//  E X T E R N A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  D E F I N E S
//******************************************************************************
//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y) gm_Print(x,y)
#else
#define msg(x,y)
#endif

#if (FEATURE_VWD == ENABLE)

#define VWD_CHANGE_THRD    3

#define VWD_HIGHLIGHT_DEMO

#define BLACK_BORDER_FIX   0        // Fix the detection with black borders in desktop with dither.

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
static gmt_PCInputConfig VwdConfig;
static gmt_VideoInfo VwdDetectInfo, VwdCoarseInfo;
static gmt_MultiVideoInfo CoarseMVWInfo;

static BYTE VWDCurrState = UNKNOWN_IMAGE;

static BYTE MVWON;
static BYTE SelectRectext;
static BYTE ExtSelectWR;

static BOOL VwdEnable = FALSE;
static WORD VwdXStart = 0, VwdYStart = 0, VwdXEnd = 0, VwdYEnd = 0;
#ifdef _DEBUG_MSG
static DWORD VwdRunTime;
#endif

#ifdef VWD_HIGHLIGHT_DEMO
static WORD VwdWinTransitCnt = 0;
#endif

#ifndef UserPrefVwdDemo
BYTE VwdDemoTemp = VWD_DEF;//Hyb20130424//VWD_DEMO_FULL;  // Temp declair
#define UserPrefVwdDemo VwdDemoTemp
#endif

//*****************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//*****************************************************************************
extern void PixCrCoarseRunEdge_Test(void);
extern void PixCrCoarseRunAcc_Test(void);
extern void PixCrCoarseRun_Test(void);
extern BOOL gm_IsAfrSet(gmt_PHY_CH Channel);

#if (FEATURE_ACM_3D == ENABLE)
extern void gm_ACM3DInit( void );
#endif

#if(FEATURE_VWD == ENABLE)
BYTE ChangeByVWD=0;//20120618 Add by KevinChen
#endif
//*****************************************************************************
//  C O D E
//*****************************************************************************
void CscUpdateColorsVWD(void)//20120618 Add by KevinChen
{
	gmt_ColorStruct st_ca;
	//BYTE B_ImageScheme;
	BYTE B_ColorTemp;
	WORD W_Contrast;
	WORD W_Saturation;
	SWORD sW_Hue;
//gm_Print("A",0);
	//B_ImageScheme = UserPrefGraphicsImageMode;
	B_ColorTemp = UserPrefPresetColorTemp;
	W_Contrast = UserPrefContrast_Vwd;
	W_Saturation = UserPrefSaturation_Vwd;		
	sW_Hue = UserPrefHue_Vwd;
	st_ca.sW_Hue = ConvertHue(sW_Hue);
	st_ca.W_Saturation = ConvertSaturation(W_Saturation);
	st_ca.W_Contrast = ConvertContrast(W_Contrast, CH_A);	
	st_ca.sB_Brightness = 0;
			
	if(IsPresetMode_GRAPHICS_CUSTOM_COLOR)
	{			
		st_ca.W_RedGain = ConvertRGBGain(UserPrefUserRedGain);
		st_ca.W_GreenGain = ConvertRGBGain(UserPrefUserGreenGain);
		st_ca.W_BlueGain = ConvertRGBGain(UserPrefUserBlueGain);		
	
		//120718 Edward correct structure and extend range to 1000 ~ -1000 for more obvious
		st_ca.sB_RedOff = (SWORD)(UserPrefUserRedOff*USER_RGB_OFFSET_SHFIT);
		st_ca.sB_GreenOff = (SWORD)(UserPrefUserGreenOff*USER_RGB_OFFSET_SHFIT);
		st_ca.sB_BlueOff = (SWORD)(UserPrefUserBlueOff*USER_RGB_OFFSET_SHFIT);
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
				case CTEMP_USER:	// YPbPr Movie				
					st_ca.W_RedGain = UserPrefUserRedGain;
					st_ca.W_GreenGain = UserPrefUserGreenGain;
					st_ca.W_BlueGain = UserPrefUserBlueGain;
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

	// OVP 3x3 matrix update
	ChangeByVWD=1;
	gm_CscUpdateColors(CH_A,&st_ca);
	ChangeByVWD=0;
}

void CscUpdateColorsVWDMovie(void)//20120914 Add by WilliamL
{
	gmt_ColorStruct st_ca;
	/*WORD W_Contrast;
	WORD W_Saturation;
	SWORD sW_Hue;
	W_Contrast = UserPrefContrast_Vwd;
	W_Saturation = UserPrefSaturation_Vwd;
	sW_Hue = UserPrefHue_Vwd;*/
	st_ca.sW_Hue = 0;//ConvertHue(sW_Hue);
	st_ca.W_Saturation = ConvertSaturation(WB_SATURATION_DEFAULT);
	st_ca.W_Contrast = ConvertContrast(WB_CONTRAST_DEFAULT, CH_A);	
	st_ca.sB_Brightness = 0;
			
	st_ca.sB_RedOff = 0x0;
	st_ca.sB_GreenOff = 0x0;
	st_ca.sB_BlueOff = 0x0;
	st_ca.W_RedGain = UserPrefColorT9300_R;
	st_ca.W_GreenGain = UserPrefColorT9300_G;
	st_ca.W_BlueGain = UserPrefColorT9300_B;

	// OVP 3x3 matrix update
	ChangeByVWD=1;
	gm_CscUpdateColors(CH_A,&st_ca);
	ChangeByVWD=0;

    gm_Print("#####CscUpdateColorsVWDMovie",0);
}

void CscUpdateColorsVWD_Advanced(void) //KevinL 20120921 updated color setting
{
	gmt_ColorStruct st_ca;
	WORD W_Contrast;
	WORD W_Saturation;
	SWORD sW_Hue;
	W_Contrast = UserPrefContrast_Vwd;
	W_Saturation = UserPrefSaturation_Vwd;		
	sW_Hue = UserPrefHue_Vwd;
	st_ca.sW_Hue = ConvertHue(sW_Hue);
	st_ca.W_Saturation = ConvertSaturation(W_Saturation);
	st_ca.W_Contrast = ConvertContrast(W_Contrast, CH_A);	
	st_ca.sB_Brightness = 0;

			
	st_ca.sB_RedOff = 0x0;
	st_ca.sB_GreenOff = 0x0;
	st_ca.sB_BlueOff = 0x0;
	st_ca.W_RedGain = UserPrefColorT7200_R;
	st_ca.W_GreenGain = UserPrefColorT7200_G;
	st_ca.W_BlueGain = UserPrefColorT7200_B;

	// OVP 3x3 matrix update
	ChangeByVWD=1;
	gm_CscUpdateColors(CH_A,&st_ca);
	ChangeByVWD=0;

    gm_Print("#####CscUpdateColorsVWD_Advanced",0);
}

//*****************************************************************************
//  Vwd Initial
//*****************************************************************************
void VWD_Initial(void)
{
   VwdConfig.DEBUGONOFF      = 1;   // 1:use following settings , 0:use default settings

   VwdConfig.BLACKONOFF      = 0;
   VwdConfig.BORDERLESSONOFF = 1 ;   // 0: Border mode , 1: Borderless mode

   VwdConfig.COA_EDGEHWTHR   = 34;
   VwdConfig.COA_EDGEDIVFACT = 20;
   VwdConfig.COA_MOTIONTHR   = 0;
   VwdConfig.COA_MOTIONGATE  = 0;
   VwdConfig.COA_REALTHR     = 70;
   VwdConfig.COA_REALGATE    = 0;
   VwdConfig.COA_BLACKTHR    = 8;
   VwdConfig.COA_FINALGATEMOTION = 0;
   VwdConfig.COA_FINALGATE   = 0;
   VwdConfig.COA_FINALMAPSEL = 0;
   VwdConfig.COA_RECTWIDTH   = 5;
   VwdConfig.COA_RECTHEIGHT  = 5;
   VwdConfig.FIN_MOTIONTHR   = 0;
   VwdConfig.FIN_BLACKTHR    = 12;
   // BLACK_BORDER_FIX
   VwdConfig.FORCE_DITHER = FALSE;
   VwdDetectInfo.VideoType = UNKNOWN_IMAGE;
   
   SelectRectext = 0;
   ExtSelectWR   = 0;

   gm_Print("VWD Gen3: May, 2012",0);
}

//*****************************************************************************
//  Clear VWD window information
//*****************************************************************************
void VWD_ClearWindowsInfo(void)
{
   VwdXStart = 0;
   VwdXEnd   = 0;
   VwdYStart = 0;
   VwdYEnd   = 0;

   VwdDetectInfo.VideoType = UNKNOWN_IMAGE;
}

//*****************************************************************************
//  Is VWD Detected
//*****************************************************************************
BOOL IsVwdDetected(void)
{
   if (VwdDetectInfo.VideoType < UNKNOWN_IMAGE)
      return TRUE;
   else
      return FALSE;
}

//*****************************************************************************
//*****************************************************************************
void Vwd_Start_Test(BYTE TestMode)
{
   PixCrAnalyzeImage(&VwdDetectInfo, &VwdCoarseInfo,
                     START_ANALYZE, VwdConfig,
                     &VWDCurrState, &CoarseMVWInfo,
                     &MVWON, SelectRectext, ExtSelectWR);

   if (TestMode == 0)
      PixCrCoarseRunEdge_Test();      // EDGE
   else if (TestMode == 1)
      PixCrCoarseRun_Test();       // REALNESS
   else // if (TestMode == 2)
      PixCrCoarseRunAcc_Test();
}
#ifdef VWD_HIGHLIGHT_DEMO
//*****************************************************************************
//  Vwd Highlight window
//*****************************************************************************
void VWD_WindowEnhance(void)
{
   WORD wTransVal;

   if ((UserPrefVwdDemo != VWD_DEMO_HIGHLIGHT) || (IsVwdDetected() == FALSE) || (VwdWinTransitCnt == 0))
      return;

   wTransVal = (WORD)((DWORD)VwdWinTransitCnt * 256 / VWD_HIGHLIGHT_DEMO_COUNT);
   UserPrefSaturation = wTransVal;

   CscUpdateColors(gmvb_MainChannel);

   VwdWinTransitCnt--;
}
#endif

//*****************************************************************************
//  Vwd Start
//*****************************************************************************
void VWD_Switch(void)
{
   if (UserPrefVwdFlag == ON)
      VWD_Start();
   else
   {
      VWD_Stop();
      PixCrDisable();
   }
}

//*****************************************************************************
//  Vwd Start
//*****************************************************************************
void VWD_Start(void)
{
#if BLACK_BORDER_FIX
   WORD W_DE_WIDTH = gm_ReadRegWord(IMP_IBD_HWIDTH);
   WORD W_DE_HSTART = gm_ReadRegWord(IMP_IBD_HSTART);
   WORD W_DE_HEIGHT = gm_ReadRegWord(IMP_IBD_VLENGTH);
   WORD W_DE_VSTART = gm_ReadRegWord(IMP_IBD_VSTART);
#endif

   if ((UserPrefVwdFlag == OFF) || (VwdEnable == TRUE))
      return;

#if (FEATURE_ACR == ENABLE)
   if (UserPrefDCR == ON)
      PixCrAnalyzeACRStop();
#endif

   // Only for digital ports
   if (IsInputDigital(gmvb_MainChannel) == FALSE)
      return;

   msg("PixCR Analysis - Start",0);

#if BLACK_BORDER_FIX
   // Check input image with black border or not.
   gm_ClearRegBitsWord(IMP_IBD_CONTROL, IMP_MEASURE_DE_nDATA);
   gm_Delay1ms(50);   
   {
      WORD W_DATA_WIDTH = gm_ReadRegWord(IMP_IBD_HWIDTH);
      WORD W_DATA_HSTART = gm_ReadRegWord(IMP_IBD_HSTART);
      WORD W_DATA_HEIGHT = gm_ReadRegWord(IMP_IBD_VLENGTH);
      WORD W_DATA_VSTART = gm_ReadRegWord(IMP_IBD_VSTART);
      WORD W_HSTART_DIFF = abs(W_DATA_HSTART - W_DE_HSTART);
      WORD W_VSTART_DIFF = abs(W_DATA_VSTART - W_DE_VSTART);

      gm_SetRegBitsWord(IMP_IBD_CONTROL, IMP_MEASURE_DE_nDATA);

      msg("W_DE_WIDTH === %d",W_DE_WIDTH);
      msg("W_DATA_WIDTH === %d",W_DATA_WIDTH);
      
      if (((W_HSTART_DIFF >= (W_DE_WIDTH)/32) && (abs(W_DATA_WIDTH - W_DE_WIDTH) >= (W_HSTART_DIFF * 2)))
            || ((W_VSTART_DIFF >= (W_DE_HEIGHT)/30) && (abs(W_DATA_HEIGHT - W_DE_HEIGHT) >= (W_VSTART_DIFF * 2))))
      {
         msg("  Shrink mode --- Force Dither mode",0);
         VwdConfig.FORCE_DITHER = TRUE;
         gm_TimerStart(VWD_DITHER_CHECK_100ms_TMR, 300); // Force DITHER for 30 sec
      }
      else
         VwdConfig.FORCE_DITHER = FALSE;
   }
#endif
   
   PixCrAnalyzeImage(&VwdDetectInfo, &VwdCoarseInfo,
                     START_ANALYZE, VwdConfig,
                     &VWDCurrState, &CoarseMVWInfo,
                     &MVWON, SelectRectext, ExtSelectWR);
#ifdef _DEBUG_MSG
   VwdRunTime = gm_GetSystemTime();
#endif
   ExtSelectWR = 0;
   VwdEnable = TRUE;
}

//*****************************************************************************
//  VWD Stop
//*****************************************************************************
void VWD_Stop(void)
{
   if (VwdEnable == FALSE)
      return;

   msg("PixCR Analysis - Stop",0);

   VWD_ClearHighlighWindow();
   VWD_ClearWindowsInfo();

#if BLACK_BORDER_FIX
   VwdConfig.FORCE_DITHER = FALSE;
   gm_TimerStop(VWD_DITHER_CHECK_100ms_TMR);
#endif

   PixCrAnalyzeImage(&VwdDetectInfo, &VwdCoarseInfo,
                     STOP_ANALYZE, VwdConfig,
                     &VWDCurrState, &CoarseMVWInfo,
                     &MVWON, SelectRectext, ExtSelectWR);

   VwdEnable = FALSE;
#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
   b_IsVWDFullWindowChecking = FALSE;
   b_IsVWDFullWindowCheckRequired = TRUE;
   gm_TimerStop(VWD_FULL_WIDNOW_1S_TMR);
#endif     

	UserPrefVWD_OSD_Enable = 0; //20120926 yvonne for not detect VWD should not enable Advance OSD	//owen 120918

}

//*****************************************************************************
//  Is VWD Enabled
//*****************************************************************************
BOOL IsVWDEnabled(void)
{
   return VwdEnable;
}

#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 	
void VWD_Full_Window_Handler(void)
{
    if ((UserPrefVwdFlag == OFF) || (!IsVWDEnabled()))
        return;

    if (gm_IsAfrSet(gmvb_MainChannel))
        return;
     
    if (gm_TimerCheck(VWD_FULL_WIDNOW_1S_TMR) != TIMER_TMO) 
		return;

	gm_TimerStop(VWD_FULL_WIDNOW_1S_TMR);
	VWD_ApplyHighLightWindow();
	b_IsVWDFullWindowChecking = FALSE;	
    b_IsVWDFullWindowCheckRequired = FALSE;	
}
#endif	


//*****************************************************************************
//  VWD_Handler
//*****************************************************************************
void VWD_Handler(void)
{
if(UserPrefFactoryFlag == TRUE)
	return; // chunbao in factory mode disable VWD

#if (FEATURE_AUDIO == ENABLE)
   if (SYS_API_AUDIO_Is_DAC_VolumeSetupDone() != TRUE)
   {
      // let audio volume control is finished and then start to deal with VWD feature
      return;
   }
#endif

   if ((UserPrefVwdFlag == OFF) || (!IsVWDEnabled()))
      return;

   if (gm_IsAfrSet(gmvb_MainChannel))
      return;

   VWD_Running();

#if BLACK_BORDER_FIX
   if (gm_TimerCheck(VWD_DITHER_CHECK_100ms_TMR) == TIMER_TMO)
   {
      if (IsVwdDetected() == FALSE)
      {
         VwdConfig.FORCE_DITHER = FALSE;
         //msg("  FORCE_DITHER -- OFF",0);
      }
   }
#endif
   
#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 	
    VWD_Full_Window_Handler();
#endif

#ifdef VWD_HIGHLIGHT_DEMO
   VWD_WindowEnhance();
#endif
}

//*****************************************************************************
//  Vwd Running
//*****************************************************************************
void VWD_Running(void)
{
   static WORD prevCoarseXStart = 0, prevCoarseYStart = 0, prevCoarseXEnd = 0, prevCoarseYEnd = 0;

   gmt_VideoType PrevVideoType;
   BYTE prevVwdState;
   BYTE RectCount = 0;
   BYTE sloop;

   SelectRectext = 0;

   prevVwdState = VWDCurrState;
   PrevVideoType = VwdDetectInfo.VideoType;

   PixCrAnalyzeImage(&VwdDetectInfo, &VwdCoarseInfo, POLL_ANALYZE, VwdConfig,
                     &VWDCurrState, &CoarseMVWInfo, &MVWON, SelectRectext, ExtSelectWR);

   if ((VwdDetectInfo.VideoType >= UNKNOWN_IMAGE) && (PrevVideoType <= SQUARE_IMAGE))
   {
      msg("VWD Lost!",0);
	  UserPrefVWD_OSD_Enable = 0;//owen 120918
      OSDRequestMsg(gmvb_MainChannel, MTO_VWD_Lost, REQUEST_NOW);//owen 20120917 
      VWD_ClearWindowsInfo();
      VWD_ClearHighlighWindow();
      
   #if BLACK_BORDER_FIX
      if ((VwdConfig.FORCE_DITHER) && (gm_TimerCheck(VWD_DITHER_CHECK_100ms_TMR) != TIMER_OK))
      {
         VwdConfig.FORCE_DITHER = FALSE;
         //msg("  FORCE_DITHER -- OFF",0);
      }
   #endif

    #ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
      b_IsVWDFullWindowChecking = FALSE;
      b_IsVWDFullWindowCheckRequired = TRUE;
      gm_TimerStop(VWD_FULL_WIDNOW_1S_TMR);
    #endif  
    #ifdef _DEBUG_MSG
      VwdRunTime = gm_GetSystemTime();
#endif
      return;
   }

#if 1
   if ((prevVwdState == COARSE_DETECTION) && (VWDCurrState == COARSE_MVWSELECT) && (MVWON == 1))
   {
      for (sloop = 4; sloop >0; sloop--)
      {
         if (((CoarseMVWInfo.Rect[sloop-1].VideoType == PERFECT_RECTFOUND)
               || (CoarseMVWInfo.Rect[sloop-1].VideoType == CUT_RECTFOUND)))
         {
            RectCount++;
            SelectRectext = sloop - 1;

            msg(" Highlight - %d",SelectRectext);
         }
      }
   }
#endif

   if (RectCount != 0)
      ExtSelectWR = 1;
   else
      ExtSelectWR = 0;

   if (VwdDetectInfo.VideoType >= UNKNOWN_IMAGE)
      return;

   if (((prevCoarseXStart != VwdCoarseInfo.XStart)
         || (prevCoarseXEnd   != VwdCoarseInfo.XEnd)
         || (prevCoarseYStart  != VwdCoarseInfo.YStart)
         || (prevCoarseYEnd != VwdCoarseInfo.YEnd)))
   {
      UserPrefVWD_OSD_Enable = 0;//owen 20120917
      prevCoarseXStart = VwdCoarseInfo.XStart;
      prevCoarseXEnd  = VwdCoarseInfo.XEnd;
      prevCoarseYStart = VwdCoarseInfo.YStart;
      prevCoarseYEnd = VwdCoarseInfo.YEnd;

      msg("VWD COARSE Detected <%d>",VwdDetectInfo.VideoType);
      msg("  XStart = %d", prevCoarseXStart);
      msg("  XEnd   = %d", prevCoarseXEnd);
      msg("  YStart = %d", prevCoarseYStart);
      msg("  YEnd   = %d", prevCoarseYEnd);
   }

   if ((abs(VwdDetectInfo.XStart - VwdXStart) > VWD_CHANGE_THRD) ||
         (abs(VwdDetectInfo.XEnd - VwdXEnd) > VWD_CHANGE_THRD) ||
         (abs(VwdDetectInfo.YStart - VwdYStart) > VWD_CHANGE_THRD) ||
         (abs(VwdDetectInfo.YEnd - VwdYEnd) > VWD_CHANGE_THRD))
   {
      VwdXStart = VwdDetectInfo.XStart;
      VwdXEnd = VwdDetectInfo.XEnd;
      VwdYStart = VwdDetectInfo.YStart;
      VwdYEnd = VwdDetectInfo.YEnd;
#ifdef _DEBUG_MSG
      msg("VWD FINE Detected <%d>",VwdDetectInfo.VideoType);
      msg("  XStart = %d", VwdXStart);
      msg("  XEnd   = %d", VwdXEnd);
      msg("  YStart = %d", VwdYStart);
      msg("  YEnd   = %d", VwdYEnd);
      msg("< Run Time :: %d ms >", gm_GetSystemTime() - VwdRunTime);
#endif
      VWD_ApplyHighLightWindow();
	UserPrefVWD_OSD_Enable = 1;
      OSDRequestMsg(gmvb_MainChannel, MTO_VWD_Enable, REQUEST_NOW);//owen 20120917 VWD Enable
    #if 0 //KevinL 20120921 move it to VWD_ApplyHighLightWindow() for general case.  
      if(UserPrefVwdDemo == VWD_DEMO_FULL)//20120914 Add by WilliamL
      {
         CscUpdateColorsVWDMovie();
   }
    #endif  
   }   
   
}

//*****************************************************************************
//  VWD_ApplyHighLightWindow
//*****************************************************************************
void VWD_ApplyHighLightWindow(void)
{
   WORD wXStart, wWidth, wYStart, wHeight;
   WORD inputWidth, inputHeight, outputWidth, outputHeight;

#ifdef DELL_U2410F
   if (UserPrefVwdFlag == VWD_DEMO_OFF)//110805 Edward for CMI
      return;
#endif   
   
   inputWidth   = stInput[gmvb_MainChannel].Hactive;
   inputHeight  = stInput[gmvb_MainChannel].Vactive;
   outputWidth  = gmvw_OutputHActive[gmvb_MainChannel];
   outputHeight = gmvw_OutputVActive[gmvb_MainChannel];

   // Increase by 2 to improve slight extension of VWD on Top & left edge of the window.
   wXStart = (WORD)(((DWORD)(VwdXStart + 2) * (DWORD)outputWidth + (DWORD)(inputWidth/2))/(DWORD)inputWidth);
   wWidth = (WORD)(((DWORD)VwdXEnd * (DWORD)outputWidth + (DWORD)(inputWidth/2))/(DWORD)inputWidth) - wXStart;
   wYStart = (WORD)(((DWORD)(VwdYStart + 2) * (DWORD)outputHeight + (DWORD)(inputHeight/2))/(DWORD)inputHeight);
   wHeight = (WORD)(((DWORD)VwdYEnd * (DWORD)outputHeight + (DWORD)(inputHeight/2))/(DWORD)inputHeight) - wYStart;

   if ((wWidth == 0) || (wHeight == 0))
      return;

#if 0 //20120927 yvonne disable for reset Display setting Movie/Game will reset saturation to default. 
      if (UserPrefVwdDemo!=VWD_DEMO_ADVANCE)//20120618 Add by KevinChen
   UserPrefSaturation = WB_SATURATION_DEFAULT;
#endif

   CscUpdateColors(gmvb_MainChannel);

   #if (FEATURE_ACM_3D == ENABLE)   
   //ACM3DSetColorScheme(VWD, UserPrefBlueStretch, UserPrefFleshToneComp);
   if(UserPrefVwdDemo==VWD_DEMO_ADVANCE)//20120618 Add by KevinChen
   	  ACM3DSetColorScheme(OFF_ACM3D_SCHEME);
   else
   ACM3DSetColorScheme(VWD_ACM3D_SCHEME);
   gm_ACM3DInit();

   // to inform Library about ACM status
   //gm_UpdateACM3DColorScheme(VWD);
   if(UserPrefVwdDemo==VWD_DEMO_ADVANCE)//20120618 Add by KevinChen
   	  gm_UpdateACM3DColorScheme(OFF_ACM3D_SCHEME);
   else
   gm_UpdateACM3DColorScheme(VWD_ACM3D_SCHEME);
#endif

#ifdef VWD_INSIDE_WINDOW_APPLY_ONLY //add for apply coefficient in second window only
   ChangeByVWD=1;
   SetupColorSpace(gmvb_MainChannel);
   ChangeByVWD=0;
#else
   SetupColorSpace(gmvb_MainChannel);
#endif

   #if 1 //KevinL 20120920 do not set DEMO video window for DELL, it will cause the screen displayed diffenect on top/bottom.(DEMO_WINDOW_SPLIT : VIDEO_WIN_UP)
   if(!IsVwdDetected())
   {
      gm_Print("VWD WIN not Detected!",0);
      gm_ClearRegBitsWord(VIDEO_WIN_CTRL, VIDEO_WIN_EN);
      gm_WriteRegWord (VIDEO_WIN_HWIDTH, PanelWidth); 
      gm_WriteRegWord (VIDEO_WIN_VLENGTH, PanelHeight);
   }
   else //Window detected, set video window with exact size later
   {
      gm_Print("VWD WIN Detected!",0);
      //gm_CscSetVideoWindow(DEMO_WINDOW_SPLIT);       
   }
   #else
   gm_CscSetVideoWindow(DEMO_WINDOW_SPLIT);
   #endif 
   
   gm_Print(" VWD - Applying",0);
   gm_Print("  wXStart = %d",wXStart);
   gm_Print("  wWidth  = %d",wWidth);
   gm_Print("  wYStart = %d",wYStart);
   gm_Print("  wHeight = %d",wHeight);
   
#ifdef VWD_FULL_WINDOW_CHECK //KevinL 20120922 
   if(((wWidth>(PanelWidth - 100)) && (wHeight>(PanelHeight- 100))) && (IsVwdDetected()))
   {    
     if((b_IsVWDFullWindowChecking == FALSE) && (b_IsVWDFullWindowCheckRequired == TRUE))
     {
        b_IsVWDFullWindowChecking = TRUE;
        gm_TimerStart(VWD_FULL_WIDNOW_1S_TMR, 8);//12); //PC power on/off Video motion 3~4 Sec. ;PS3 power on, video motion ~10 Sec. 
        return;
     }
   }
   else
   {
        b_IsVWDFullWindowChecking = FALSE;
        b_IsVWDFullWindowCheckRequired = TRUE;
        gm_TimerStop(VWD_FULL_WIDNOW_1S_TMR);
   }
#endif

   if (UserPrefVwdDemo == VWD_DEMO_RIGHT)
   {
      gm_WriteRegWord(VIDEO_WIN_HSTART, wXStart + (wWidth / 2));
      gm_WriteRegWord(VIDEO_WIN_VSTART, wYStart);
      gm_WriteRegWord(VIDEO_WIN_HWIDTH, wWidth / 2);
   }
   else if (UserPrefVwdDemo == VWD_DEMO_LEFT)
   {
      gm_WriteRegWord(VIDEO_WIN_HSTART, wXStart);
      gm_WriteRegWord(VIDEO_WIN_VSTART, wYStart);
      gm_WriteRegWord(VIDEO_WIN_HWIDTH, wWidth / 2);
   }
   else //if (UserPrefVwdDemo == VWD_DEMO_FULL)
   {
      gm_WriteRegWord(VIDEO_WIN_HSTART, wXStart);
      gm_WriteRegWord(VIDEO_WIN_VSTART, wYStart);
      gm_WriteRegWord(VIDEO_WIN_HWIDTH, wWidth);
#ifdef VWD_HIGHLIGHT_DEMO
      if (UserPrefVwdDemo == VWD_DEMO_HIGHLIGHT)
      {
         VwdWinTransitCnt = VWD_HIGHLIGHT_DEMO_COUNT;
      }
#endif
   }

#if 1 //KevinL 20120920 update color when VWD detected
   if((UserPrefVwdDemo == VWD_DEMO_ADVANCE) && IsVwdDetected()) 
#else
   if(UserPrefVwdDemo == VWD_DEMO_ADVANCE)//20120618 Add by KevinChen
#endif   
   {//gm_Print("call cscupdatecolors VWD",0);
   	  //CscUpdateColorsVWD();
   	  CscUpdateColorsVWD_Advanced(); //KevinL 20120921 updated color setting
   }
   else if((UserPrefVwdDemo == VWD_DEMO_FULL) && IsVwdDetected()) //KevinL 20120921 VWD Movie color setting
   {
       CscUpdateColorsVWDMovie();
   }

   gm_WriteRegWord(VIDEO_WIN_VLENGTH, wHeight);

   if(UserPrefVwdDemo == VWD_DEMO_ADVANCE)//20120618 Add by KevinChen
   {
      BYTE SharpnessVWD;
      SharpnessVWD=ConvertSharpnessVWD(UserPrefSharpness_Vwd);
      //gm_Print("SharpnessVWD - %d",SharpnessVWD);
      gm_WriteRegWord(MHF_DEMOWIND_SHARPNESS, SharpnessVWD|(SharpnessVWD<<8));
      gm_WriteRegWord(MVF_DEMOWIND_SHARPNESS, SharpnessVWD|(SharpnessVWD<<8));
   }
   else
   {
   // Set the Demo Sharpness
   #ifdef DELL_U2410F
    	gm_WriteRegWord(MHF_DEMOWIND_SHARPNESS, 0x0000); // 20120716 KimChye
   		gm_WriteRegWord(MVF_DEMOWIND_SHARPNESS, 0x0000); // 20120716 KimChye
   #else  
   gm_WriteRegWord(MHF_DEMOWIND_SHARPNESS, 0x7F7F);
   gm_WriteRegWord(MVF_DEMOWIND_SHARPNESS, 0x7F7F);
   #endif
   }
   
   // Config Sharpness Demo Window
   if(stInput[gmvb_MainChannel].Hactive <= PanelWidth)
   {
   gm_WriteRegWord(MHF_DEMOWIND_CTRL_2, gm_ReadRegWord(VIDEO_WIN_VSTART)); // 12-bit   
   gm_WriteRegWord(MHF_DEMOWIND_CTRL_3, gm_ReadRegWord(VIDEO_WIN_VLENGTH)+gm_ReadRegWord(VIDEO_WIN_VSTART)); // 12-bit
   }
   else
   {
      DWORD DW_Temp = 0;

      DW_Temp = (DWORD)gm_ReadRegWord(VIDEO_WIN_VSTART)*stInput[gmvb_MainChannel].Vactive/PanelHeight;      
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_2, (WORD)DW_Temp); // 12-bit   

      DW_Temp = DW_Temp + (DWORD)gm_ReadRegWord(VIDEO_WIN_VLENGTH)*stInput[gmvb_MainChannel].Vactive/PanelHeight;
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_3, (WORD)DW_Temp); // 12-bit   
   }

   gm_WriteRegWord(MVF_DEMOWIND_CTRL_2, gm_ReadRegWord(VIDEO_WIN_VSTART)); // 12-bit   
   gm_WriteRegWord(MVF_DEMOWIND_CTRL_3, gm_ReadRegWord(VIDEO_WIN_VLENGTH)+gm_ReadRegWord(VIDEO_WIN_VSTART)); // 12-bit   
   
   {//This is just a close setting not match one...
      DWORD HScale = gm_GetHScaleValue(gmvb_MainChannel);
      BYTE B_Offset = 0;
      DWORD DW_HTemp;
      #define NON_SCALE_OFFSET 24 // This value is tested with 1920, 1680 and 1600 panel.

      B_Offset = (BYTE)((HScale * NON_SCALE_OFFSET)/1048576L)+1; // 2^20 = 1048576

      DW_HTemp = (DWORD)gm_ReadRegWord(VIDEO_WIN_HSTART)*stInput[gmvb_MainChannel].Hactive/PanelWidth;

      // MVF become a fix value when the input is equal or bigger than panel
      if(stInput[gmvb_MainChannel].Hactive < PanelWidth)
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_0, ((DW_HTemp + 3) << 1)|WINDOW_SHARP_EN); // 13-bit
      else
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_0, ((gm_ReadRegWord(VIDEO_WIN_HSTART) + 3) << 1)|WINDOW_SHARP_EN); // 13-bit
         
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_0, ((DW_HTemp + B_Offset + 1)<< 1)|WINDOW_SHARP_EN); // 13-bit

      DW_HTemp = DW_HTemp + (DWORD)gm_ReadRegWord(VIDEO_WIN_HWIDTH)*stInput[gmvb_MainChannel].Hactive/PanelWidth;
      
      // MVF become a fix value when the input is equal or bigger than panel
      if(stInput[gmvb_MainChannel].Hactive < PanelWidth)
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_1, DW_HTemp+4); // 13-bit    
      else
         gm_WriteRegWord(MVF_DEMOWIND_CTRL_1, gm_ReadRegWord(VIDEO_WIN_HSTART) + gm_ReadRegWord(VIDEO_WIN_HWIDTH) +4); // 13-bit    
         
      gm_WriteRegWord(MHF_DEMOWIND_CTRL_1, DW_HTemp+B_Offset); // 13-bit
   }
   if(IsVwdDetected()) //KevinL 20120920 do not enable video window if VWD is not detected, otherwise color setting may be applied incorrectlly. 
   {
   gm_SetRegBitsByte(VIDEO_WIN_CTRL, VIDEO_WIN_EN);
   }
   gm_ClearAndSetBitsWord(COLOR_MATRIX_CTRL, MAIN_MATRIX_WIN_CTRL, 2 << MAIN_MATRIX_WIN_CTRL_SHIFT);

   gm_Delay1ms(10);  // TEST ONLY
   gm_SetRegBitsByte(OVP_UPDATE_CTRL, OVP_FORCE_UPDATE);
   gm_SyncUpdateWait(gmvb_MainChannel,INPUT_OUTPUT_CLOCK);

#if (FEATURE_ACC3 == ENABLE)
   if (gm_IsACCEnabled())
      gm_ACC3UpdateModeDependants();
#endif
}

void VWD_ClearHighlighWindow(void)
{
#ifdef VWD_ACM_TEST
	#if (FEATURE_ACM_3D == ENABLE) //20121025 Kordonm+ for Exit VWD, doesn't close the ACC/ACM window
			AdjustACM3DColorScheme();
	#endif
	#ifdef DELL_U2410F//120209 Edward copy from 2410
			AdjustDELLACCScheme();
	#endif
	//UserPrefSaturation = WB_SATURATION_DEFAULT; //20120908 Disable by KevinChen for Saturation setting value when power saving. 
   CscUpdateColors(gmvb_MainChannel);
   gm_CscSetVideoWindow(DEMO_WINDOW_SPLIT);
#endif

if(IS_DCR_ON())
 gm_ClearAndSetBitsWord(ACC_CTRL, ACC_WIN_CTRL, 0X03);//owen 121215 for DCR on sometime can not work 

#ifdef DELL_U2410F//120813 Edward change for JP suggest do not disable video window or system will hang up when DCR get ACC from RAM
	//20121001 yvonne disable condition for windows not clear,contrast can not adjust issue
	//if(!IS_DCR_ON()|| IsPresetMode_GRAPHICS_GAME)//120926 Edward for contrast can not adjust issue in the "Game" mode
	gm_WriteRegWord (VIDEO_WIN_CTRL, 0);
	
   gm_WriteRegWord (VIDEO_WIN_HWIDTH, PanelWidth); 
   gm_WriteRegWord (VIDEO_WIN_VLENGTH, PanelHeight);
#else
   gm_WriteRegWord (VIDEO_WIN_CTRL, 0);
   gm_WriteRegWord (VIDEO_WIN_HWIDTH, 0);
   gm_WriteRegWord (VIDEO_WIN_VLENGTH, 0);
#endif

   // Clear Demo Sharpness
   gm_ClearRegBitsWord(MHF_DEMOWIND_CTRL_0, WINDOW_SHARP_EN);
   gm_ClearRegBitsWord(MVF_DEMOWIND_CTRL_0, WINDOW_SHARP_EN);
   

   gm_SetRegBitsByte(OVP_UPDATE_CTRL, OVP_FORCE_UPDATE);
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);
}
void AdjustAdvanceVWD(void)//20120618 Add by KevinChen
{
	static WORD Contrast_Vwd_Last;
	static BYTE Sharpness_Vwd_Last;
	static SBYTE Offset_Vwd_Last;
	static SWORD Hue_Vwd_Last;
	static WORD Saturation_Vwd_Last;

	//gm_Print("UserPrefContrast_Vwd-%d",UserPrefContrast_Vwd);
	//gm_Print("UserPrefSharpness_Vwd-%d",UserPrefSharpness_Vwd);
	//gm_Print("UserPrefOffset_Vwd-%d",UserPrefOffset_Vwd);
	//gm_Print("UserPrefHue_Vwd-%d",UserPrefHue_Vwd);
	//gm_Print("UserPrefSaturation_Vwd-%d",UserPrefSaturation_Vwd);

	//if((Contrast_Vwd_Last!=UserPrefContrast_Vwd)||(Hue_Vwd_Last!=UserPrefHue_Vwd)||(Saturation_Vwd_Last!=UserPrefSaturation_Vwd)||(Offset_Vwd_Last!=UserPrefOffset_Vwd))
	{
		Contrast_Vwd_Last=UserPrefContrast_Vwd;
		Hue_Vwd_Last=UserPrefHue_Vwd;
		Saturation_Vwd_Last=UserPrefSaturation_Vwd;
		Offset_Vwd_Last=UserPrefOffset_Vwd;
		CscUpdateColorsVWD();
	}

	if(Sharpness_Vwd_Last!=UserPrefSharpness_Vwd)
	{
		BYTE SharpnessVWD;
		Sharpness_Vwd_Last=UserPrefSharpness_Vwd;
		//gm_Print("UserPrefSharpness_Vwd - %d",UserPrefSharpness_Vwd);
		SharpnessVWD=ConvertSharpnessVWD(UserPrefSharpness_Vwd);
		//gm_Print("SharpnessVWD - %d",SharpnessVWD);
		gm_WriteRegWord(MHF_DEMOWIND_SHARPNESS, SharpnessVWD|(SharpnessVWD<<8));
		gm_WriteRegWord(MVF_DEMOWIND_SHARPNESS, SharpnessVWD|(SharpnessVWD<<8));
	}

	if((gm_ReadRegWord(COLOR_MATRIX_CTRL)&&MAIN_MATRIX_WIN_CTRL)!=0x20)
	{
		gm_SetRegBitsByte(VIDEO_WIN_CTRL, VIDEO_WIN_EN);
		gm_ClearAndSetBitsWord(COLOR_MATRIX_CTRL, MAIN_MATRIX_WIN_CTRL, 2 << MAIN_MATRIX_WIN_CTRL_SHIFT);

	}
}
#else
BOOL IsVwdDetected(void){return gmd_FALSE;}
BOOL IsVWDEnabled(void){return gmd_FALSE;}
void VWD_Start(void){}
void VWD_Stop(void){}
void VWD_ClearHighlighWindow(void){}
void VWD_Switch(void) {}
void VWD_ApplyHighLightWindow(void) {}
void AdjustAdvanceVWD(void){}//20120618 Add by KevinChen

#endif
