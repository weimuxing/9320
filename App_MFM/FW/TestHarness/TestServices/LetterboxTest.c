/*
	$Workfile:   LetterboxTest.c  $
	$Revision: 1.6 $
	$Date: 2012/01/16 06:31:02 $
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
// MODULE:      LetterboxTest.c
//
// USAGE:       Utility functions for test bench related letterbox detection
//			    functions
//
// NOTE:		Overall performance
//				Run Appstest 37
//
//				Customized window size and place
//				Run appstest 40 param0 param1 param2 param3 param4 param5
//								param6 param7
//				Note:
//				param0 is Top Window horizontal start point
//				param1 is Top Window horizontal end point
//				param2 is Top Window vertical start point
//				param3 is Top Window vertical end point
//				param4 is Bottom Window horizontal start point
//				param5 is Bottom Window horizontal end point
//				param6 is Bottom Window vertical start point
//				param7 is Bottom Window vertical end point
//
//				Customized noise threshold
//				Run appstest 41 param0
//
//				Customized line number +/- lines to make the change
//				Run appstest 42 param0
//
//				Customized zoom and shrink counter number (the time to zoom and shrink)
//				Run apsstest 43 param0 param1
//				Note: param0 is zoom counter value. Param2 is shrink counter value. Param0 is great larger than param2.
//
//				16 bin historgram value print out
//				Run appstest 44
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  E X T E R N A L     F U N C T I O N S
//******************************************************************************
#ifdef USE_LETTERBOX_DETECTION

//extern WORD gmp_DecNoiseMeasFilter(BYTE);

//******************************************************************************
//  G L O B A L     F U N C T I O N S
//******************************************************************************

void LtbxDetSetup		(void);
void LtbxDetSetup1		(void);
void LtbxDetSetup2		(void);
void LtbxDetSetup3		(void);
void LtbxDetSetup4		(void);
void LtbxTest			(void);
void LtbxDetNoiseMeter	(void);
void LtbxDetInvalidSetup(void);

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define IMP_LTBX_DETECT_THRESH_VAL		0x0500	// Detection threshold set to 5
#define IMP_LTBX_FILTER_SIZE_VAL		0x0090	// filter size set to 9
#define IMP_LTBX_LINE_THRESH_VAL		0x0500	// Line threshold set to 5

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

static WORD LTBX_TEST_ENABLED = 0;
static WORD FirstLine;
static WORD LastLine;
static WORD ActiveRegion2;    		//previous value
static WORD ActiveRegion1 = 0;		//new value
static WORD LastLine2;
static WORD LastLine1 = 0;
static WORD FirstTime = 0;
static WORD DiffFromPrev, DiffFromPrev_thresh;
static WORD count1, count2, counter1_thresh, counter2_thresh, time;
// count1 will 1000, count2 will 60
static BYTE Ratio;

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void LtbxDetSetup(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetSetup(void)
{
   WORD gNoise, noiseThresh, IMP_LTBX_NOISE_THRESH_VAL, i;

   for (i=0;i<200;i++)
   {
      gNoise = gmp_DecNoiseMeasFilter(1);

   }
   gNoise = gmp_DecNoiseMeasFilter(1);
   noiseThresh = gNoise/4;
   IMP_LTBX_NOISE_THRESH_VAL = noiseThresh;


   gm_WriteRegWord(IMP_LTBX_TOP_H_START,		gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_TOP_H_END,			gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_TOP_V_START,		gmvw_InputVStartOrg[gmvb_MainChannel]);
   gm_WriteRegWord(IMP_LTBX_TOP_V_END,			gmvw_InputVStartOrg[gmvb_MainChannel]+80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START,	gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END,		gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START,	gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]-80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END,		gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]);
   // Sync Update pending registers
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_WriteRegWord(IMP_LTBX_CONFIG,		IMP_LTBX_LINE_THRESH_VAL |
                   IMP_LTBX_NOISE_THRESH_VAL);
   gm_WriteRegWord(IMP_LTBX_CONTROL,	IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_FILTER_SIZE_VAL|
                   IMP_LTBX_YUV_EN|
                   IMP_LTBX_DET_EN);
   LTBX_TEST_ENABLED = 1;
}

//******************************************************************************
//
// FUNCTION       : void LtbxDetSetup1(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
#if 0//def USER_EXTENDED_GPROBE_CMD
void LtbxDetSetup1(void)  //window size, place
{
   WORD gNoise, noiseThresh, IMP_LTBX_NOISE_THRESH_VAL, i;
   WORD W_Param0, W_Param1, W_Param2, W_Param3, W_Param4, W_Param5,W_Param6, W_Param7;

   for (i=0;i<200;i++)
   {
      gNoise = gmp_DecNoiseMeasFilter(1);

   }
   gNoise = gmp_DecNoiseMeasFilter(1);
   noiseThresh = gNoise/4;
   IMP_LTBX_NOISE_THRESH_VAL = noiseThresh;

   W_Param0 = (WORD)GetParam(0);
   W_Param1 = (WORD)GetParam(1);
   W_Param2 = (WORD)GetParam(2);
   W_Param3 = (WORD)GetParam(3);
   W_Param4 = (WORD)GetParam(4);
   W_Param5 = (WORD)GetParam(5);
   W_Param6 = (WORD)GetParam(6);
   W_Param7 = (WORD)GetParam(7);

   gm_WriteRegWord(IMP_LTBX_TOP_H_START,		W_Param0);
   gm_WriteRegWord(IMP_LTBX_TOP_H_END,			W_Param1);
   gm_WriteRegWord(IMP_LTBX_TOP_V_START,		W_Param2);
   gm_WriteRegWord(IMP_LTBX_TOP_V_END,			W_Param3);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START,	W_Param4);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END,		W_Param5);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START,	W_Param6);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END,		W_Param7);
   // Sync Update pending registers
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_WriteRegWord(IMP_LTBX_CONFIG,		IMP_LTBX_LINE_THRESH_VAL |
                   IMP_LTBX_NOISE_THRESH_VAL);
   gm_WriteRegWord(IMP_LTBX_CONTROL,	IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_FILTER_SIZE_VAL|
                   IMP_LTBX_YUV_EN|
                   IMP_LTBX_DET_EN);
   LTBX_TEST_ENABLED = 1;
}


//******************************************************************************
//
// FUNCTION       : void LtbxDetSetup2(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetSetup2(void)     //noise threshold
{
   WORD IMP_LTBX_NOISE_THRESH_VAL;

   WORD W_Param0 = (WORD)GetParam(0);
   IMP_LTBX_NOISE_THRESH_VAL = W_Param0;

   gm_WriteRegWord(IMP_LTBX_TOP_H_START,		gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_TOP_H_END,			gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_TOP_V_START,		gmvw_InputVStartOrg[gmvb_MainChannel]);
   gm_WriteRegWord(IMP_LTBX_TOP_V_END,			gmvw_InputVStartOrg[gmvb_MainChannel]+80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START,	gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END,		gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START,	gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]-80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END,		gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]);
   // Sync Update pending registers
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_WriteRegWord(IMP_LTBX_CONFIG,		IMP_LTBX_LINE_THRESH_VAL |
                   IMP_LTBX_NOISE_THRESH_VAL);
   gm_WriteRegWord(IMP_LTBX_CONTROL,	IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_FILTER_SIZE_VAL|
                   IMP_LTBX_YUV_EN|
                   IMP_LTBX_DET_EN);
   LTBX_TEST_ENABLED = 1;
}

//******************************************************************************
//
// FUNCTION       : void LtbxDetSetup3(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetSetup3(void)        //line numbers to make change
{
   WORD gNoise, noiseThresh, IMP_LTBX_NOISE_THRESH_VAL, i;
   WORD W_Param0 = (WORD)GetParam(0);

   DiffFromPrev_thresh = W_Param0;

   for (i=0;i<200;i++)
   {
      gNoise = gmp_DecNoiseMeasFilter(1);

   }
   gNoise = gmp_DecNoiseMeasFilter(1);
   noiseThresh = gNoise/4;
   IMP_LTBX_NOISE_THRESH_VAL = noiseThresh;


   gm_WriteRegWord(IMP_LTBX_TOP_H_START,		gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_TOP_H_END,			gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_TOP_V_START,		gmvw_InputVStartOrg[gmvb_MainChannel]);
   gm_WriteRegWord(IMP_LTBX_TOP_V_END,			gmvw_InputVStartOrg[gmvb_MainChannel]+80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START,	gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END,		gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START,	gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]-80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END,		gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]);
   // Sync Update pending registers
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_WriteRegWord(IMP_LTBX_CONFIG,		IMP_LTBX_LINE_THRESH_VAL |
                   IMP_LTBX_NOISE_THRESH_VAL);
   gm_WriteRegWord(IMP_LTBX_CONTROL,	IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_FILTER_SIZE_VAL|
                   IMP_LTBX_YUV_EN|
                   IMP_LTBX_DET_EN);
   LTBX_TEST_ENABLED = 1;
}


//******************************************************************************
//
// FUNCTION       : void LtbxDetSetup4(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetSetup4(void)      //counter1, counter2
{
   WORD gNoise, noiseThresh, IMP_LTBX_NOISE_THRESH_VAL, i;

   WORD W_Param0 = (WORD)GetParam(0);
   WORD W_Param1 = (WORD)GetParam(1);
   counter1_thresh = W_Param0;
   counter2_thresh = W_Param1;
   for (i=0;i<200;i++)
   {
      gNoise = gmp_DecNoiseMeasFilter(1);

   }
   gNoise = gmp_DecNoiseMeasFilter(1);
   noiseThresh = gNoise/4;
   IMP_LTBX_NOISE_THRESH_VAL = noiseThresh;


   gm_WriteRegWord(IMP_LTBX_TOP_H_START,		gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_TOP_H_END,			gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_TOP_V_START,		gmvw_InputVStartOrg[gmvb_MainChannel]);
   gm_WriteRegWord(IMP_LTBX_TOP_V_END,			gmvw_InputVStartOrg[gmvb_MainChannel]+80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START,	gmvw_InputHStartOrg[gmvb_MainChannel]+gmvw_InputHActiveOrg[gmvb_MainChannel]/6);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END,		gmvw_InputHStartOrg[gmvb_MainChannel]+2*(gmvw_InputHActiveOrg[gmvb_MainChannel]/3));
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START,	gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]-80);
   gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END,		gmvw_InputVStartOrg[gmvb_MainChannel]+gmvw_InputVActiveOrg[gmvb_MainChannel]);
   // Sync Update pending registers
   gm_SyncUpdateWait(gmvb_MainChannel, INPUT_OUTPUT_CLOCK);

   gm_WriteRegWord(IMP_LTBX_CONFIG,		IMP_LTBX_LINE_THRESH_VAL |
                   IMP_LTBX_NOISE_THRESH_VAL);
   gm_WriteRegWord(IMP_LTBX_CONTROL,	IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_DETECT_THRESH_VAL|
                   IMP_LTBX_FILTER_SIZE_VAL|
                   IMP_LTBX_YUV_EN|
                   IMP_LTBX_DET_EN);
   LTBX_TEST_ENABLED = 1;
}
#endif

//******************************************************************************
//
// FUNCTION       : void LtbxTest(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxTest(void)
{
   if (LTBX_TEST_ENABLED == 1)
   {
//  	DynamicScalerStart();
      gm_InputWaitVsync(gmvb_MainChannel, 1);
      FirstLine = gm_ReadRegWord(IMP_LTBX_FIRST_ACTIVE_LINE);
      LastLine = gm_ReadRegWord(IMP_LTBX_LAST_ACTIVE_LINE);

      if ((FirstLine != 0x7FF) && (LastLine != 0x7FF))
      {
         ActiveRegion2 = LastLine - FirstLine;
         LastLine2 = LastLine;

         if (FirstTime == 0)
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            FirstTime = 1;
            count1 = 0;
            count2 = 0;
            time = 0;
         }

         if (ActiveRegion2 > ActiveRegion1)
         {
            DiffFromPrev = ActiveRegion2 - ActiveRegion1;
         }
         else
         {
            DiffFromPrev = ActiveRegion1 - ActiveRegion2;
         }

         if ((DiffFromPrev > 20))       //June20, change to 10 to prevent lord of ring bottom bouncing
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            //       count1 = 0;        //can't reset to 0 because "ARmageddon" movie constantly change
            //       count2 = 0;        //from 137 to 193. If reset, will never do scaling.
            time = 0;
         }
         else
         {
            if (gm_IsInterlaced(gmvb_MainChannel))
               Ratio = (7200)/(2*ActiveRegion1);
            else
               Ratio = (7200)/(ActiveRegion1);

            //zoom case  zoom first time

            if ((Ratio >= 15)&&(count1 ==1000)&&(ActiveRegion1 >= 130) && (ActiveRegion1 < 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               if ((DiffFromPrev >20)|(time==0))  //line+/- 5 lines diff
               {
                  // Reconfiguration
                  gm_SetupInputMain();
                  gm_ScalerDynamic(1);
                  //	gm_ScalerDynamicWait(4);
                  gm_ScalerDynamicWait(2);
                  DynamicScalerStop();
                  gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
                  count1 = 0;
                  count2 = 0;
                  time++;
               }
            }

            //shrink case
            if ((Ratio >= 15)&&(count2 ==60)&&(ActiveRegion1 >= 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               // Reconfiguration

               gm_SetupInputMain();
               gm_ScalerDynamic(1);
               //				gm_ScalerDynamicWait(4);
               gm_ScalerDynamicWait(2);
               DynamicScalerStop();

               gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
               count2 = 0;
               count1 = 0;
            }
         }

         if (count1 < 1000)   //zoom 1000 case
         {
            count1 ++;
         }
         if (count2 < 60)   //shrink case
         {
            count2 ++;
         }
      }    //first&last line !=0x7FF
   }    // LTBX_TEST_EN
}  //void

//******************************************************************************
//
// FUNCTION       : void LtbxTest1(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxTest1(void)         //counter1,counter2
{
   if (LTBX_TEST_ENABLED == 1)
   {
//  DynamicScalerStart();
      gm_InputWaitVsync(gmvb_MainChannel, 1);
      FirstLine = gm_ReadRegWord(IMP_LTBX_FIRST_ACTIVE_LINE);
      LastLine = gm_ReadRegWord(IMP_LTBX_LAST_ACTIVE_LINE);

      if ((FirstLine != 0x7FF) && (LastLine != 0x7FF))
      {
         ActiveRegion2 = LastLine - FirstLine;
         LastLine2 = LastLine;

         if (FirstTime == 0)
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            FirstTime = 1;
            count1 = 0;
            count2 = 0;
            time = 0;
         }

         if (ActiveRegion2 > ActiveRegion1)
         {
            DiffFromPrev = ActiveRegion2 - ActiveRegion1;
         }
         else
         {
            DiffFromPrev = ActiveRegion1 - ActiveRegion2;
         }

         if ((DiffFromPrev > 20))       //June20, change to 10 to prevent lord of ring bottom bouncing
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            //       count1 = 0;        //can't reset to 0 because "ARmageddon" movie constantly change
            //       count2 = 0;        //from 137 to 193. If reset, will never do scaling.
            time = 0;
         }
         else
         {

            if (gm_IsInterlaced(gmvb_MainChannel))
               Ratio = (7200)/(2*ActiveRegion1);
            else
               Ratio = (7200)/(ActiveRegion1);

            //zoom case  zoom first time

            if ((Ratio >= 15)&&(count1 ==counter1_thresh)&&(ActiveRegion1 >= 130) && (ActiveRegion1 < 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               if ((DiffFromPrev >20)|(time==0))  //line+/- 5 lines diff
               {
                  // Reconfiguration
                  gm_SetupInputMain();
                  gm_ScalerDynamic(1);
                  //	gm_ScalerDynamicWait(4);
                  gm_ScalerDynamicWait(2);
                  DynamicScalerStop();
                  gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
                  count1 = 0;
                  count2 = 0;
                  time++;
               }
            }

            //shrink case
            if ((Ratio >= 15)&&(count2 ==counter2_thresh)&&(ActiveRegion1 >= 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               // Reconfiguration

               gm_SetupInputMain();
               gm_ScalerDynamic(1);
               //				gm_ScalerDynamicWait(4);
               gm_ScalerDynamicWait(2);
               DynamicScalerStop();

               gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
               count2 = 0;
               count1 = 0;
            }
         }

         if (count1 < 1000)   //zoom 1000 case
         {
            count1 ++;
         }
         if (count2 < 60)   //shrink case
         {
            count2 ++;
         }

      }    //first&last line !=0x7FF
   }    // LTBX_TEST_EN
}  //void

//******************************************************************************
//
// FUNCTION       : void LtbxTest2(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxTest2(void)     //line numbers
{
   if (LTBX_TEST_ENABLED == 1)
   {
//  DynamicScalerStart();
      gm_InputWaitVsync(gmvb_MainChannel, 1);
      FirstLine = gm_ReadRegWord(IMP_LTBX_FIRST_ACTIVE_LINE);
      LastLine = gm_ReadRegWord(IMP_LTBX_LAST_ACTIVE_LINE);

      if ((FirstLine != 0x7FF) && (LastLine != 0x7FF))
      {
         ActiveRegion2 = LastLine - FirstLine;
         LastLine2 = LastLine;

         if (FirstTime == 0)
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            FirstTime = 1;
            count1 = 0;
            count2 = 0;
            time = 0;
         }

         if (ActiveRegion2 > ActiveRegion1)
         {
            DiffFromPrev = ActiveRegion2 - ActiveRegion1;
         }
         else
         {
            DiffFromPrev = ActiveRegion1 - ActiveRegion2;
         }

         if ((DiffFromPrev > DiffFromPrev_thresh))       //June20, change to 10 to prevent lord of ring bottom bouncing
         {
            ActiveRegion1 = ActiveRegion2;
            LastLine1 = LastLine2;
            //       count1 = 0;        //can't reset to 0 because "ARmageddon" movie constantly change
            //       count2 = 0;        //from 137 to 193. If reset, will never do scaling.
            time = 0;
         }
         else
         {

            if (gm_IsInterlaced(gmvb_MainChannel))
               Ratio = (7200)/(2*ActiveRegion1);
            else
               Ratio = (7200)/(ActiveRegion1);

            //zoom case  zoom first time

            if ((Ratio >= 15)&&(count1 ==1000)&&(ActiveRegion1 >= 130) && (ActiveRegion1 < 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               if ((DiffFromPrev >DiffFromPrev_thresh)|(time==0))  //line+/- 5 lines diff
               {
                  // Reconfiguration
                  gm_SetupInputMain();
                  gm_ScalerDynamic(1);
                  //	gm_ScalerDynamicWait(4);
                  gm_ScalerDynamicWait(2);
                  DynamicScalerStop();
                  gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
                  count1 = 0;
                  count2 = 0;
                  time++;
               }
            }

            //shrink case
            if ((Ratio >= 15)&&(count2 ==60)&&(ActiveRegion1 >= 200))
            {
               stInput[gmvb_MainChannel].Vstart	= FirstLine;
               stInput[gmvb_MainChannel].Vactive	= ActiveRegion1;

               // Reconfiguration

               gm_SetupInputMain();
               gm_ScalerDynamic(1);
               //				gm_ScalerDynamicWait(4);
               gm_ScalerDynamicWait(2);
               DynamicScalerStop();

               gm_AfmSetup(W_OverscanEnabled[gmvb_MainChannel]);
               count2 = 0;
               count1 = 0;
            }
         }

         if (count1 < 1000)   //zoom 1000 case
         {
            count1 ++;
         }
         if (count2 < 60)   //shrink case
         {
            count2 ++;
         }

      }    //first&last line !=0x7FF
   }    // LTBX_TEST_EN
}  //void



//******************************************************************************
//
// FUNCTION       : void LtbxDetNoiseMeter(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetNoiseMeter(void)
{
#if 0
   WORD firstL, lastL, noiseMeter1, noiseMeter2, noiseMeter3,i;

   firstL = gm_ReadRegWord(IMP_LTBX_FIRST_ACTIVE_LINE);
   lastL = gm_ReadRegWord(IMP_LTBX_LAST_ACTIVE_LINE);

   for (i=0;i<200;i++)
   {
      noiseMeter1 = gmp_DecNoiseMeasFilter(1);
      noiseMeter2 = gmp_DecNoiseMeasFilter(2);
      noiseMeter3 = gmp_DecNoiseMeasFilter(3);
   }
   gm_Print("fL = %d", firstL);
   gm_Print("lL = %d", lastL);
   gm_Print("global noise %d", noiseMeter1);
   gm_Print("global noise %d mV", noiseMeter2);
   gm_Print("global noise %d dB", noiseMeter3);
#endif
}


//******************************************************************************
//
// FUNCTION       : void LtbxDetInvalidSetup(void)
//
// USAGE          :
//
// INPUT          : None
//
// OUTPUT         : None
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
void LtbxDetInvalidSetup(void)
{
   WORD firstL, lastL, noiseThresh, value, firstLine, lastLine;

   for (noiseThresh = 0; noiseThresh < 0x50; noiseThresh++)
   {
      gm_WriteRegWord(IMP_LTBX_CONTROL, 0x593);

      gm_WriteRegWord(IMP_LTBX_TOP_H_START, 0x12A);
      //  gm_WriteRegWord(IMP_LTBX_TOP_H_END, 0x292);
      gm_WriteRegWord(IMP_LTBX_TOP_H_END, 0x200);
      gm_WriteRegWord(IMP_LTBX_TOP_V_START, 0x17);
      gm_WriteRegWord(IMP_LTBX_TOP_V_END, 0x67);

      gm_WriteRegWord(IMP_LTBX_BOTTOM_H_START, 0x12A);
      //gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END, 0x292);
      gm_WriteRegWord(IMP_LTBX_BOTTOM_H_END, 0x200);
      gm_WriteRegWord(IMP_LTBX_BOTTOM_V_START, 0xB3);
      gm_WriteRegWord(IMP_LTBX_BOTTOM_V_END, 0x103);

      value = 0x500 + noiseThresh;
      gm_WriteRegWord(IMP_LTBX_CONFIG, value);
      gm_WriteRegWord(HOST_CONTROL, 0xFF);

      firstLine = gm_ReadRegWord(IMP_LTBX_FIRST_ACTIVE_LINE);
      lastLine = gm_ReadRegWord(IMP_LTBX_LAST_ACTIVE_LINE);

      if (((firstLine== 0x7FF)|(lastLine==0x7FF))|((firstLine== 0x0)|(lastLine==0x0)))
      {
         //    gm_Print("firstLine = %d", firstLine);
         //    gm_Print("lastLine = %d", lastLine);    //still wrong because you need a lot of times
         gm_Print("noise threshold is %x", value);
      }
   }
}

#endif // USE_LETTERBOX_DETECTION

//*********************************  END  **************************************

