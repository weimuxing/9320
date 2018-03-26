/*
	$Workfile:   DebugHandler.c  $
	$Revision: 1.24 $
	$Date: 2013/04/02 05:06:02 $
*/
#define __DEBUGHANDLER_C__
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
// MODULE:      DebugHandler.c
//
// USAGE:       This module contains debug handler for application and library
//				debug messages.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include <string.h>
#include "System\all.h"
#include "StdLibInc\gm_Register.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define MAIN_LOOP_LIMIT		10			// Normal Main loop limit in ms unit
// If exist, debug message will be
// printed
//#define AVERAGE_TIME					// Average time within DEBUG_MESSAGE_TMO
//#define INDIVIDUAL_HANDLER_TIME		// Individual handler time
#define MONITOR_STACK_USAGE				// Check stack usage
#define DEBUG_PRINT_CURRENT_PORT
//#define DEBUG_RECORD

#define STACK_START		0x8000


//******************************************************************************
//  G L O B A L     V A R I A B L E
//******************************************************************************
//#define DEBUG_HANDLER
#ifdef DEBUG_HANDLER
WORD W_HandlerPrevTime[MAX_DBG_HDLR];		// Handler time
WORD W_HandlerMaxTime[MAX_DBG_HDLR];		// Handler time
#endif

#ifdef DEBUG_MODE_SETUP
DWORD D_Elapsed[4];
#endif

#ifdef DEBUG_VIDEO_PROCESSING
WORD  W_LastVideoProcessing = 0;
#endif

#ifdef MONITOR_STACK_USAGE
#define STACK_SIZE		1024
#define STACK_COMPARE	16

WORD W_StackMark=STACK_START;		// Current stack top mark
ROM  BYTE Ba_StackZero[STACK_COMPARE]=
{
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
    0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55
};
#endif

#ifdef DEBUG_RECORD
WORD far *pW_DebugRecordBuf = NULL_PTR;
WORD W_DebugRecordSize;
WORD W_DebugRecordIndex;
#endif
//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  S T A T I C     V A R I A B L E
//******************************************************************************

#ifdef DEBUG_HANDLER
static WORD W_PrevTime, W_CurrentTime;		// System time
static WORD W_MaxTime;						// System time
static BOOL B_FirstTime;					// First time flag
static BOOL B_MainLoopLimit;				// Main loop time flag
#endif

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void DebugHandlerInit(void)
//
// USAGE          : Debug handler initilization
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
void DebugHandlerInit(void)
{
#ifdef DEBUG_HANDLER
    gm_TimerStart(DEBUG_100ms_TMR, DEBUG_MESSAGE_TMO);	// Debug handler timer
    B_FirstTime = TRUE;									// Set flag
    B_MainLoopLimit = FALSE;							//
#endif
}

//******************************************************************************
//
// FUNCTION       : void DebugHandler(void)
//
// USAGE          : Handler all the application and library debug message for
//                  FW development.
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
void DebugHandler(void)
{
#ifdef DEBUG_HANDLER
    WORD i;

#ifdef MONITOR_STACK_USAGE
    WORD W_Status, W_Count;
    BYTE *Bp_Addr;
#endif

#ifdef DEBUG_BUFFER_ON
    PrintFromBufferHex();
#endif

#ifdef VIEW_VBI_TTX_DATA
    dumpVBIData();
#endif

    //
    // Get system time after a main loop
    // Monitor main loop time usage
    //
    if (B_FirstTime == TRUE)
    {
        B_FirstTime = FALSE;					// Clear flag
        W_PrevTime = gm_GetSystemTimeW();		// Current system time
    }
    else
    {
        W_CurrentTime = gm_GetSystemTimeW();  	// Current system time
        if (W_PrevTime > W_CurrentTime)		   	// Overflow ?
            W_CurrentTime = (0xFFFF - W_PrevTime) + W_CurrentTime + 1;	// Time elasped
        else
            W_CurrentTime -= W_PrevTime;	   	// Time elasped
        //
        if (W_CurrentTime > MAIN_LOOP_LIMIT)   	// Over limit
        {
            B_MainLoopLimit = TRUE;			   	// Set flag
            if (W_CurrentTime > W_MaxTime)		//
                W_MaxTime = W_CurrentTime;		// Save MAX loop time
        }
    }

    //
    // Set timer control to print error/warning/debug message
    //
    if (gm_TimerCheck(DEBUG_100ms_TMR) == TIMER_TMO)
    {
        gm_TimerStart(DEBUG_100ms_TMR, DEBUG_MESSAGE_TMO);	// Re-start timer

        if (B_MainLoopLimit == TRUE)
        {
            gm_Print("Main loop over limit - %d",W_MaxTime);
            B_MainLoopLimit = FALSE;
            W_MaxTime = 0;

#ifdef INDIVIDUAL_HANDLER_TIME
            for (i=0; i<MAX_DBG_HDLR; i++)
            {
                gm_Printf("Handler - %d = %d",i,W_HandlerMaxTime[i]);
                W_HandlerMaxTime[i]=0;
            }
#endif
        }

        //
        // Monitor current stack position
        //
#ifdef MONITOR_STACK_USAGE
        Bp_Addr = W_StackMark;							// Current stack
        for (W_Count=0; W_Count<STACK_SIZE; W_Count++)	// Check for 1KB stack
        {
            if (_fmemcmp(Bp_Addr,Ba_StackZero,STACK_COMPARE) == 0)
                // All zeros
                break;
            else
                Bp_Addr -= STACK_COMPARE;				// Move down the stack
        }
        if (Bp_Addr < W_StackMark)           	// Stack move ?
        {										// Yes
            W_StackMark = Bp_Addr;              // Save new value
            gm_Print("Current max stack size - %d",STACK_START - W_StackMark);
        }
#endif
    }
    //
    // Current system time for next loop
    //
    W_PrevTime = gm_GetSystemTimeW();
#endif

#ifdef DEBUG_PRINT_CURRENT_PORT
{
   static BYTE pre_Mian=0xFF, pre_Pip=0xFF;
   if ((pre_Mian!=UserPrefCurrentInputMain)||(pre_Pip!=UserPrefCurrentInputPip))
   {
      gm_Print("--->UserPrefCurrentInputMain=%d",UserPrefCurrentInputMain);
      gm_Print("--->UserPrefCurrentInputPip=%d",UserPrefCurrentInputPip);
      pre_Mian=UserPrefCurrentInputMain;
      pre_Pip=UserPrefCurrentInputPip;
   }
}
#endif
}

//******************************************************************************
//
// FUNCTION       : void MeasureHandlerStart(DBG_HDLR W_Handler)
//
// USAGE          : Mark the beginning of the handler to measure
//
// INPUT          : W_Handler = hander number in debughandler.h
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
#ifdef DEBUG_HANDLER
void MeasureHandlerStart(DBG_HDLR W_Handler)
{
    if (W_Handler < MAX_DBG_HDLR)
        W_HandlerPrevTime[W_Handler] = gm_GetSystemTimeW();
    // Current system time
    W_Handler++;							// Dummy
}
#endif

//******************************************************************************
//
// FUNCTION       : void MeasureHandlerEnd(DBG_HDLR W_Handler)
//
// USAGE          : Mark the end of the handler to measure
//
// INPUT          : W_Handler = hander number in debughandler.h
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
#ifdef DEBUG_HANDLER
void MeasureHandlerEnd(DBG_HDLR W_Handler)
{
    WORD W_Time, W_Prev;

    if (W_Handler < MAX_DBG_HDLR)
    {
        W_Time = gm_GetSystemTimeW();  		// Current system time
        W_Prev = W_HandlerPrevTime[W_Handler];	//
        //
        if (W_Prev > W_Time)		   			// Overflow ?
            W_Time = (0xFFFF - W_Prev)+W_Time+1;	// Time elasped
        else
            W_Time -= W_Prev;	   				// Time elasped

#ifdef AVERAGE_TIME
        if (W_HandlerMaxTime[W_Handler] == 0)
            W_HandlerMaxTime[W_Handler] = W_Time;
        else
            W_HandlerMaxTime[W_Handler]=(W_Time+W_HandlerMaxTime[W_Handler])/2;
#else
        if (W_Time > W_HandlerMaxTime[W_Handler])
        {
            W_HandlerMaxTime[W_Handler] = W_Time;
        }
#endif
    }
    W_Handler++;							// Dummy
}
#endif

//******************************************************************************
//
// FUNCTION       : void DebugChannel(gmt_PHY_CH B_Channel)
//
// USAGE          : Debug - Displays Channel Details
//
// INPUT          : B_Channel -CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_InputModeAttr				(RO)
//                  gmvw_InputHTotal				(RO)
//                  st_InputMeasurement.W_VTotal	(RO)
//                  st_InputMeasurement.W_HFreq					(RO)
//                  st_InputMeasurement.W_VFreq					(RO)
//                  gmvb_ChannelSource				(RO)
//                  W_OverscanEnabled				(RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef DEBUG_MODE_SETUP
void DebugChannel(gmt_PHY_CH B_Channel)
{
    WORD W_Data;
    gm_Printf("", 0);
    if (B_Channel == gmvb_MainChannel)
        gm_Printf("*** M A I N *********************************", 0);
    else
        gm_Printf("*** P I P ***********************************", 0);

    if (gm_IsValidSignal(B_Channel))
    {
        W_Data = stInput[B_Channel].ModeFlags;
        gm_Printf("------------------ Input --------------------", 0);
        if (gm_IsGraphics(B_Channel) == TRUE)
            gm_Printf("   Mode Attrs (0..1) : Graphics", 0);
        else if (gm_IsSDVideo(B_Channel) == TRUE)
            gm_Printf("   Mode Attrs (0..1) : SD Video", 0);
        else if (gm_IsHDVideo(B_Channel) == TRUE)
            gm_Printf("   Mode Attrs (0..1) : HD Video", 0);
        else if (gm_IsEDVideo(B_Channel) == TRUE)
            gm_Printf("   Mode Attrs (0..1) : ED Video", 0);
        else
            gm_Printf("   Mode Attrs (0..1) : Invalid", 0);
        gm_Printf("      Interlaced (2) : %d", (W_Data & gmd_MODE_INTERLACED) >> 2);
        gm_Printf("       Neg HSync (3) : %d", (W_Data & gmd_MODE_NEG_HSYNC) >> 3);
        gm_Printf("       Neg VSync (4) : %d", (W_Data & gmd_MODE_NEG_VSYNC) >> 4);
        gm_Printf("             SOG (5) : %d", (W_Data & gmd_MODE_SOG) >> 5);
        gm_Printf("           CSync (6) : %d", (W_Data & gmd_MODE_CSYNC) >> 6);
        gm_Printf(" Overlap_640_720 (7) : %d", (W_Data & gmd_MODE_OVERLAPPED_GR640_GR720) >> 7);
        gm_Printf("    Overlap_480P (8) : %d", (W_Data & gmd_MODE_OVERLAPPED_480P) >> 8);
        gm_Printf("       Esitmated (9) : %d", (W_Data & gmd_MODE_ESTIMATED) >> 9);
        gm_Printf("   Out of Range (10) : %d", (W_Data & gmd_MODE_OUT_RANGE) >> 10);
        gm_Printf("     Sub Sample (11) : %d", (W_Data & gmd_MODE_SUB_SAMPLE) >> 11);
        gm_Printf("        Unknown (12) : %d", (W_Data & gmd_MODE_UNKNOWN) >> 12);
        gm_Printf("          Valid (13) : %d", (W_Data & gmd_MODE_VALID) >> 13);
        gm_Printf("       Complete (14) : %d", (W_Data & gmd_MODE_COMPLETE) >> 14);
        gm_Printf(" Embedded Video (15) : %d", (W_Data & gmd_MODE_EMBEDDED_VIDEO) >> 15);
        gm_Printf("         Input Total : %dx%d", stInput[B_Channel].Htotal, stInput[B_Channel].Vtotal);
        gm_Printf("         Input Start : %dx%d", stInput[B_Channel].Hstart, stInput[B_Channel].Vstart);
        gm_Printf("    Ori Input Active : %dx%d", gmvw_InputHActiveOrg[B_Channel], gmvw_InputVActiveOrg[B_Channel]);
        gm_Printf("        Input Active : %dx%d", stInput[B_Channel].Hactive, stInput[B_Channel].Vactive);
        gm_Printf("        Sync Offsets : %dx%d", gmvsw_InputHSyncOffset[B_Channel], gmvsw_InputVSyncOffset[B_Channel]);
        W_Data = stInput[B_Channel].Hfrq;
        gm_Printf("              H Freq : %d.%d KHz", (int)(W_Data / 10), (int)(W_Data % 10));
        W_Data = stInput[B_Channel].Vfrq;
        gm_Printf("              V Freq : %d.%d Hz", (int)(W_Data / 10), (int)(W_Data % 10));
        switch (gmvb_ChannelSource[B_Channel])
        {
        case DIP_A:
            gm_Printf("              Source : DIP A", 0);
            break;
//			case DIP_B:
//				gm_Printf(" 			 Source : DIP B", 0);
//				break;
        case HDMI_SRC:
            gm_Printf(" 			 Source : HDMI_SRC", 0);
            break;
        case AIP_HD_GFX:
            gm_Printf("              Source : AIP HD/GFX", 0);
            break;
        case DVI1_SRC:
            gm_Printf("              Source : DVI1_SRC", 0);
            break;
        case DP2_SRC:
            gm_Printf(" 			 Source : Display Port_2", 0);
            break;
        case DP1_SRC:
            gm_Printf(" 			 Source : Display Port_1", 0);
            break;
        case STG_TIM:
            gm_Printf(" 			 Source : STG Timing", 0);
            break;
        default:
            gm_Printf("              Source : Unknown", 0);
            break;
        }
        gm_Printf("       Over Sampling : %d", gmvw_InputOversample[B_Channel]);
        gm_Printf("    Physical Channel : %c", B_Channel == CH_A?'A':'B');
        gm_Printf("        Output Start : %dx%d", gmvw_OutputHStart[B_Channel], gmvw_OutputVStart[B_Channel]);
        gm_Printf("       Output Active : %dx%d", gmvw_OutputHActive[B_Channel], gmvw_OutputVActive[B_Channel]);
        switch (B_AspectMethod[B_Channel])
        {
        case AR_FULL_SCREEN:
            gm_Printf("              Aspect : Full Screen", 0);
            break;
        case AR_LETTER_BOX:
            gm_Printf("              Aspect : Letterbox", 0);
            break;
        case AR_PILLAR_BOX:
            gm_Printf("              Aspect : Pillar Box", 0);
            break;
        case AR_PILLAR_BOX_EXPAND:
            gm_Printf("              Aspect : Pillar Box Expand", 0);
            break;
        case AR_LETTER_BOX_EXPAND:
            gm_Printf("              Aspect : Letter Box Expand", 0);
            break;
        case AR_WATER_GLASS:
            gm_Printf("              Aspect : Water Glass", 0);
            break;
        case AR_PANORAMIC:
            gm_Printf("              Aspect : Panoramic", 0);
            break;

        default:
            gm_Printf("              Aspect : Unknown", 0);
            break;
        }
    }
    else
    {
        gm_Printf("  Invalid Signal.", 0);
    }
}
#endif

//******************************************************************************
//
// FUNCTION       : void DebugMode(gmt_PHY_CH B_Channel)
//
// USAGE          : Debug - Displays Mode Details
//
// INPUT          : B_Channel
//
// OUTPUT         : None
//
// GLOBALS        : gmv_OUTPUT_HTOTAL				(RO)
//                  gmv_OUTPUT_VTOTAL				(RO)
//                  B_DddsLockMethod				(RO)
//                  gmvb_OutputFrameRate			(RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#ifdef DEBUG_MODE_SETUP
void DebugMode(gmt_PHY_CH B_Channel)
{
    WORD	W_Data;
    DWORD	D_Hours;
    DWORD	D_Mins;
    DWORD	D_Secs;

#if 1
    // Display simplified ModeSetup Message by default.
    // For Full message, use Appstest 22 0
    if(SystemFlags.sys.PrintDebug != TRUE)
    {
    if (B_Channel == gmvb_MainChannel)
        gm_Printf("*** M A I N : %d x %d Timing Stable ***", stInput[B_Channel].Hactive, stInput[B_Channel].Vactive);
    else
        gm_Printf("*** P I P : %d x %d Timing Stable ***", stInput[B_Channel].Hactive, stInput[B_Channel].Vactive);
       return;
    }
    SystemFlags.sys.PrintDebug=FALSE;
#endif

    for (W_Data = 0; W_Data < 5; ++W_Data)
        gm_Printf("", 0);

    DebugChannel(B_Channel);

    gm_Printf("", 0);
    gm_Printf("*** O U T P U T *****************************", 0);
    gm_Printf("----------------- Display -------------------", 0);
    gm_Printf("        Output Total : %dx%d", gmv_OUTPUT_HTOTAL, gmv_OUTPUT_VTOTAL);

   {
      BYTE B_LockMode = gm_ReadRegWord(SYNC_CONTROL)&FRAME_LOCK_MODE;

      if(B_LockMode == BIT0)
         gm_Printf("           Lock Mode : Fixed Frame Lock", 0);
      else if(B_LockMode == BIT1)
         gm_Printf("           Lock Mode : Dynamic Frame Lock", 0);
      else if(B_LockMode == BIT2)
         gm_Printf("           Lock Mode : Programmed Frame Lock", 0);
      else
         gm_Printf("           Lock Mode : Free Run", 0);         
   }
   
    if (B_DisplayLockMethod == DISP_LOCK_NORMAL)
    {            
        gm_Printf("           Frequency : %d Hz (DDDS Close Loop Lock)", gmvb_OutputFrameRate);
    }
    else if (B_DisplayLockMethod == DISP_LOCK_DFL)
    {
      if((gm_ReadRegWord(LOCK_STATUS)&(DFL_PERIOD_LOCK|DFL_PHASE_LOCK)) == (DFL_PERIOD_LOCK|DFL_PHASE_LOCK))
         gm_Printf("           Frequency : %d Hz (DFL Locked)", gmvb_OutputFrameRate);
      else
         gm_Printf("           Frequency : %d Hz (DFL unLocked)", gmvb_OutputFrameRate);        
    }
    else
        gm_Printf("           Frequency : %d Hz (FreeRun-DDDS Open Loop)", gmvb_OutputFrameRate);
    
    if (gmvb_DDDSSourceChannel == gmvb_MainChannel)
        gm_Printf("                Lock : Main", 0);
    else
        gm_Printf("                Lock : PIP", 0);
    
    if (UserPrefPipMode == NO_PIP)
        gm_Printf("                 PIP : None", 0);
    else if (UserPrefPipMode == LARGE_PIP)
        gm_Printf("                 PIP : Large", 0);
    else if (UserPrefPipMode == SMALL_PIP)
        gm_Printf("                 PIP : Small", 0);
    else if (UserPrefPipMode == SIDE_BY_SIDE)
        gm_Printf("                 PIP : Side-by-Side", 0);
    else if (UserPrefPipMode == SIDE_BY_SIDE_TALL)
        gm_Printf("                 PIP : Side-by-Side-Tall", 0);
    
    gm_Printf("--------------- Processing ------------------", 0);
    DebugProcessing();
    gm_Printf("----------------- Clocks --------------------", 0);
    W_Data = gm_DdsGetFreq(CLK_SDDS);

    if (gm_ReadRegWord(SDDS1_CONTROL) & SDDS1_FORCE_OPLOOP)
        gm_Printf("                SDDS : %d.%d MHz (OpenLoop)", (int)(W_Data / 10), (int)(W_Data % 10));
    else
        gm_Printf("                SDDS : %d.%d MHz (ClosedLoop)", (int)(W_Data / 10), (int)(W_Data % 10));

    W_Data = gm_DdsGetFreq(CLK_DDDS);


    if (gm_ReadRegWord(DDDS1_CONTROL) & DDDS1_FORCE_OPLOOP)
        gm_Printf("                DDDS1 : %d.%d MHz (OpenLoop)", (int)(W_Data / 10), (int)(W_Data % 10));
    else
        gm_Printf("                DDDS1 : %d.%d MHz (ClosedLoop)", (int)(W_Data / 10), (int)(W_Data % 10));

    W_Data = gm_DdsGetFreq(CLK_VDDS1);
    gm_Printf("                VDDS1 : %d.%d MHz (%s)",
              (int)(W_Data / 10), (int)(W_Data % 10),
              (gm_ReadRegWord(AVDDS1_CONTROL0) & AVDDS1_FORCE_OPLOOP) ? "OpenLoop" : "ClosedLoop");

    W_Data = gm_DdsGetFreq(CLK_VDDS2);
    gm_Printf("                VDDS2 : %d.%d MHz (%s)",
              (int)(W_Data / 10), (int)(W_Data % 10),
              (gm_ReadRegWord(AVDDS2_CONTROL0) & AVDDS2_FORCE_OPLOOP) ? "OpenLoop" : "ClosedLoop");


#ifdef DEBUG_PROFILE
    {
        DWORD D_Data;
        gm_Printf("", 0);
        gm_Printf("*** P R O F I L I N G ***********************", 0);
        gm_Printf("---------------- Intervals ------------------", 0);
        gm_Printf("         Mode Determ : + %ld ms (From Freerun)", (D_Elapsed[1] - D_Elapsed[0]) );
        gm_Printf("          Mode Setup : + %ld ms", (D_Elapsed[2] - D_Elapsed[1]) );
        gm_Printf("            Blenders : + %ld ms", (D_Elapsed[3] - D_Elapsed[2]) );
        gm_Printf("               Total :   %ld ms", (D_Elapsed[3] - D_Elapsed[0]) );
        D_Hours = D_Elapsed[3] / (60*60*1000L);
        D_Data  = D_Elapsed[3] % (60*60*1000L);
        D_Mins  = D_Data / (60*1000L);
        D_Data  = D_Data % (60*1000L);
        D_Secs  = D_Data / (1000L);
        D_Data  = D_Data % (1000L);
        gm_Printf("         System Time :   %dh:%dm:%d.%ds",
                  (WORD)D_Hours, (WORD)D_Mins, (WORD)D_Secs, (WORD)D_Data );
    }
#endif

    gm_Printf("", 0);
    gm_Printf("-------------- ModeSetup():End ---------------", 0);
}
#endif

//******************************************************************************
//
// FUNCTION       : void DebugProcessing(void)
//
// USAGE          : Debug - Displays Video Processing Details
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvw_VideoProcessingDesired		(RO)
//                  gmvw_VideoProcessingEnabled		(RO)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
#if defined(DEBUG_MODE_SETUP) || defined(DEBUG_VIDEO_PROCESSING)
void DebugProcessing(void)
{
    char st[100];
    BOOL B_First = TRUE;

    _fstrcpy(st, (char *)"    Video Processing : ");

    if (gmvd_VideoProcessingDesired & VIDEO_MADI)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_MADI)
            _fstrcat(st, (char *)"MADI");
        else
            _fstrcat(st, (char *)"(madi)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_DCDI_MAIN)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_DCDI_MAIN)
            _fstrcat(st, (char*)"DCDI_MAIN");
        else
            _fstrcat(st, (char*)"(dcdi_main)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_FILMMODE)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_FILMMODE)
        {
            _fstrcat(st, (char*)"FILM");
            if (gm_IsFilmDetected() == TRUE)
                _fstrcat(st, (char*)"+AFM");
            else
                _fstrcat(st, (char*)"+(afm)");
        }
        else
            _fstrcat(st, (char*)"(film)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_3_3)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_3_3)
            _fstrcat(st, (char*)"3:3");
        else
            _fstrcat(st, (char*)"(3:3)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_2_2)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_2_2)
            _fstrcat(st, (char*)"2:2");
        else
            _fstrcat(st, (char*)"(2:2)");
        B_First = FALSE;
    }

#if (FEATURE_CCS == ENABLE)
    if (gmvd_VideoProcessingDesired & VIDEO_CCS)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_CCS)
            _fstrcat(st, (char*)"CCS");
        else
            _fstrcat(st, (char*)"(ccs)");
        B_First = FALSE;
    }
#endif

    if (gmvd_VideoProcessingDesired & VIDEO_TNR)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_TNR)
            _fstrcat(st, (char*)"TNR");
        else
            _fstrcat(st, (char*)"(tnr)");
        B_First = FALSE;
    }

#if (FEATURE_TNR_PROGRESSIVE == ENABLE)
    if (gmvd_VideoProcessingDesired & VIDEO_TNR_PROGRESSIVE)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingDesired & VIDEO_TNR_PROGRESSIVE)
            _fstrcat(st, (char*)"TNR_P");
        else
            _fstrcat(st, (char*)"(tnr_p)");
        B_First = FALSE;
    }
#endif

    if (gmvd_VideoProcessingDesired & VIDEO_MNR)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_MNR)
            _fstrcat(st, (char*)"MNR");
        else
            _fstrcat(st, (char*)"(mnr)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_GNR)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_GNR)
            _fstrcat(st, (char*)"GNR");
        else
            _fstrcat(st, (char*)"(gnr)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_ACC)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_ACC)
            _fstrcat(st, (char*)"ACC");
        else
            _fstrcat(st, (char*)"(acc)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_ACM)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_ACM)
            _fstrcat(st, (char*)"ACM");
        else
            _fstrcat(st, (char*)"(acm)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_HEM)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_HEM)
            _fstrcat(st, (char*)"HEM");
        else
            _fstrcat(st, (char*)"(hem)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_SPATIAL)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_SPATIAL)
            _fstrcat(st, (char*)"SPATIAL");
        else
            _fstrcat(st, (char*)"(spatial)");
        B_First = FALSE;
    }

    if (gmvd_VideoProcessingDesired & VIDEO_DYN_SCAL)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_DYN_SCAL)
            _fstrcat(st, (char*)"DYN");
        else
            _fstrcat(st, (char*)"(dyn)");
        B_First = FALSE;
    }

    //PIP DCDI
#if (FEATURE_PIP_DCDI == ENABLE)
    if (gmvd_VideoProcessingDesired & VIDEO_DCDI_PIP)
    {
        if (B_First == FALSE) _fstrcat(st, (char *)"+");
        if (gmvd_VideoProcessingEnabled & VIDEO_DCDI_PIP)
            _fstrcat(st, (char*)"DCDI_PIP");
        else
            _fstrcat(st, (char*)"(dcdi_pip)");
        B_First = FALSE;
    }
#endif
    gm_Printf(st, 0);
}
#endif

#ifdef DEBUG_RECORD
void DebugRecordInit(WORD far *DataBuf,WORD Size)
{
   pW_DebugRecordBuf = DataBuf;
   _fmemset(pW_DebugRecordBuf,0,Size*2);
   W_DebugRecordSize = Size;
}

WORD DebugRecord(WORD Index,WORD *DataBuf,WORD Size)
{
   if(Index + Size > W_DebugRecordSize)
      return Index;
   _fmemcpy((pW_DebugRecordBuf + Index),DataBuf,Size*2);
   W_DebugRecordIndex = Index + Size;
   return W_DebugRecordIndex;
}
#endif

/***********************************  END  ************************************/

