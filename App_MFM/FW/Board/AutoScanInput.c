/*
	$Workfile:   AutoScanInput.c  $
	$Revision: 1.6 $
	$Date: 2012/05/31 08:24:24 $
*/
#define __AUTOSCANINPUT_C__
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
// MODULE:      AutoScanInput.c
//
// USAGE:       This module contains function that is used for Automatic
//              Selection of physical Input Port for logical VIDEO Input
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include <mem.h>
#include "System\All.h"



//******************************************************************************
#ifdef AUTO_SCAN_INPUT
//#define AUTO_SCAN_INPUT_PIP
//Autoscan is a configuration ability to group several video input ports in one "ring".
//For example, project contains 3 rings with names "Video 1", Video 2", "DVD" and user can
//		switch between them by pressing "input select" key on remote control
//		"Video 1" ring combines "SVideo 1" and "CVBS 1" input ports and firmware "automatically"
//		select inputs which has Sync sygnal.
//
// ROM ASInputRings[AUTOSCANINPUT_RINGS_SIZE] structure is responsible for Autoscan rings configuration.
//It is one-dimensional array, but actually it represents several "rings", which are sets of IDs
//	(ALL_BOARDS_INPUT_PORT type enumerators) and every ring must contain
//		- at least one port ID
//		- one "NOT_USED" enumerator in the end as a terminator of current ring set.
//
//For example:
//		"...ALL_BOARDS_CVBS1, ALL_BOARDS_SVIDEO1, NOT_USED,..." //defines ring with 2 inputs CVBS1 and SVIDEO1
//		"...ALL_BOARDS_COMP1, NOT_USED,..." //defines ring with 1 input: COMPONENT1.
//
//First input port in the ring is default port, i.e. it will be first checked on Sync presence after
//NVRAM reset (or after user change input ring if AUTO_SCAN_INPUT_NVRAM is not defined).
//

//******************************************************************************
//  D E F I N E S
//******************************************************************************

//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg_i(x,y)		gm_Printf(x,y)
#define msg_ii(x,y,z)	gm_Printf(x,y,z)
#else
#define msg_i(x,y)
#define msg_ii(x,y,z)
#endif

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : BOOL AInputRingSearch(gmt_UserDefConnNames connection, TAInputRingRec *const ringrec)
//
// USAGE          : Search given connection thru ASInputRings array, if succeed returns its index in the array,
//							otherwise returns -1
//
// INPUT          : connection - ID of connection to search
//
// OUTPUT         : index- if found current connection in autoscan table, otherwise "-1"
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
static BYTE AInputRingSearch(gmt_UserDefConnNames connection)
{
   BYTE num;
   BYTE const far* pASInputRings = ASInputRings;
   for (num = 0; num < AUTOSCANINPUT_RINGS_SIZE; num++, pASInputRings++)
      if (*pASInputRings == connection)
         return num;
   return 0xFF;
}

//******************************************************************************
//
// FUNCTION       : void AutoScanInit(gmt_PHY_CH B_Channel)
//
// USAGE          : Initializes AutoScan timers for each channel
//
// INPUT          : B_Channel - CH_A or CH_B
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
void AutoScanInit(gmt_PHY_CH B_Channel)
{
   if (B_Channel==CH_A)
      gm_TimerStart(AUTOSCAN_CHA_10ms_TMR , MAIN_AUTOSCAN_CYCLE);
   else
      gm_TimerStart(AUTOSCAN_CHB_10ms_TMR , PIP_AUTOSCAN_CYCLE);
}

//******************************************************************************
//
// FUNCTION       : static void SelectNextVideoInput(BYTE *const pConnection)
//
// USAGE          : Returns ports next to the given inside current ring.
//							If given port is last in ring returns first one from the same ring.
//
// INPUT          : current video input port
//
// OUTPUT         : next video input port in the same ring
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
static void SelectNextVideoInput(BYTE *const pConnection)
{
   BYTE cur_num;
   if ((cur_num = AInputRingSearch(*pConnection)) != 0xFF)
   {
      BYTE num = cur_num;
      do
      {
         if (ASInputRings[++num] == NOT_USED)
         {
            while (--num != 0)
            {
               if (ASInputRings[num] == NOT_USED)
               {
                  num++;
                  break;
               }
            }
         }
         if (isCableConnected(ASInputRings[num]))
         {
            *pConnection = ASInputRings[num];
            return;
         }
		} while (num != cur_num);
   }
}

//******************************************************************************
//
// FUNCTION       : BOOL AutoScanCondition(gmt_UserDefConnNames UserPrefCurrentInput)
//
// USAGE          : Determines if autoscan should apply to the current input. I.e. if there are other
//							input ports in the same ring as given - return true, otherwise - false.
//
// INPUT          : current video input port
//
// OUTPUT         : TRUE -  if autoscan should apply to the current input
//                  FALSE - if autoscan should not apply to the current input
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
BOOL AutoScanCondition(gmt_UserDefConnNames connection)
{
   BYTE con_loc = (BYTE)connection;
   SelectNextVideoInput(&con_loc);
   return (con_loc != (BYTE)connection);
}

//******************************************************************************
//
// FUNCTION       : void AutoScanInputHandler(void)
//
// USAGE          : Periodically called from main loop to provide automatic shift of input ports inside one ring
//                  in the case of sync is lost on current input port
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel			(R)
//                  UserPrefCurrentInputMain	(R/W)
//                  UserPrefCurrentInputPip		(R/W)
//                  gmvb_ChannelSource			(R)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void AutoScanInputHandler(void)
{
   // Do not try to Auto Scan Input while Splash Screen is not completed
   if (B_SplashComplete == TRUE)
   {
      // Main channel
      if ((AutoScanCondition(UserPrefCurrentInputMain) && !gm_TimingIsStable(gmvb_MainChannel)))
      {
         if (gm_TimerCheck(AUTOSCAN_CHA_10ms_TMR + gmvb_MainChannel) == TIMER_TMO)
         {
            BYTE CurrentInputMain;
            CurrentInputMain = UserPrefCurrentInputMain;
            msg_i("AUTO_SCAN_INPUT: UserPrefCurrentInputMain = %d",UserPrefCurrentInputMain);

            SelectNextVideoInput(&UserPrefCurrentInputMain);
            // if there are port selection constraints for pip then fix the pip source too
            if(UserPrefPipMode != NO_PIP && UserPrefCurrentInputPip == UserPrefCurrentInputMain)
            {
               SelectNextVideoInput(&UserPrefCurrentInputMain);
               if(UserPrefCurrentInputPip == UserPrefCurrentInputMain)
               {
                  UserPrefCurrentInputMain = CurrentInputMain;
               }
               if(UserPrefCurrentInputMain == CurrentInputMain)
               {
                  AutoScanInit(gmvb_MainChannel);
               }
            }

            AdjustCurrentInputMain();
            msg_i("AUTO_SCAN_INPUT: Next main source is %d", UserPrefCurrentInputMain);
         }
      }

#ifdef AUTO_SCAN_INPUT_PIP
      //
      // PIP channel
      //
      if (UserPrefPipMode != NO_PIP)
      {

         // Internal - other than Hudson
         if ((AutoScanCondition(UserPrefCurrentInputPip) && !gm_TimingIsStable(gmvb_PipChannel)))
         {
            if (gm_TimerCheck(AUTOSCAN_CHA_10ms_TMR + gmvb_PipChannel) == TIMER_TMO)
            {
               BYTE CurrentInputPip;
               CurrentInputPip = UserPrefCurrentInputPip;
               msg_i("AUTO_SCAN_INPUT: UserPrefCurrentInputPip = %d", UserPrefCurrentInputPip);

               SelectNextVideoInput(&UserPrefCurrentInputPip);
               // if there are port selection constraints for pip then fix the pip source too
               if(UserPrefCurrentInputPip == UserPrefCurrentInputMain)
               {
                  SelectNextVideoInput(&UserPrefCurrentInputPip);
                  if(UserPrefCurrentInputPip == UserPrefCurrentInputMain)
                  {
                     UserPrefCurrentInputPip = CurrentInputPip;
                  }
                  if(UserPrefCurrentInputPip == CurrentInputPip)
                  {
                     AutoScanInit(gmvb_PipChannel);
                  }
               }

               AdjustCurrentInputPip();
               msg_i("AUTO_SCAN_INPUT: Next PIP source is %d", UserPrefCurrentInputPip);
            }
         }
      }
#endif
   }
}


//******************************************************************************
#ifdef AUTO_SCAN_INPUT_NVRAM

//******************************************************************************
//
// FUNCTION       : void AutoScanInput_SetDefault(gmt_PHY_CH B_Channel, gmt_UserDefConnNames CurrentPort)
//
// USAGE          : Writes the value of given video input port to NVRAM, to make it default for apropriate ring.
//
// INPUT          : B_Channel, CurrentPort
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
void AutoScanInput_SetDefault(gmt_PHY_CH B_Channel, gmt_UserDefConnNames connection)
{
   BYTE num, ring, num_in_ring;
   BOOL ring_found = FALSE;
   BYTE const far* pASInputRings = ASInputRings;
   ring= num_in_ring = 0;
   for (num = AUTOSCANINPUT_RINGS_SIZE; num; num--, pASInputRings++)
      if (*pASInputRings == NOT_USED)
      {
         if (ring_found)
            return; //FALSE
         if (num_in_ring > 1)
            ring++;
         num_in_ring = 0;
      }
      else
      {
         num_in_ring++;
         if (*pASInputRings == connection)
            ring_found = TRUE;
         if (num_in_ring > 1)
         {
            if (ring_found)
            {
               NVRam_WriteByte( AutoScanInputStart + ring*2 + B_Channel, connection );
               return; //TRUE
            }
         }
      }
}

//******************************************************************************
//
// FUNCTION       : void AutoScanInput_InitDefault( void )
//
// USAGE          : Reset default input ports in NVRAM in every ring to predefined values (first in ASInputRings)
//							Should be called when NVRAM reset is executed.
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
void AutoScanInput_InitDefault( void )
{
   BYTE num, ring, num_in_ring;
   BYTE const far* pASInputRings = ASInputRings;
   gmt_UserDefConnNames connection = NOT_USED;
   ring = num_in_ring = 0;
   for (num = AUTOSCANINPUT_RINGS_SIZE; num; num--, pASInputRings++)
   {
      if (*pASInputRings == NOT_USED)
      {
         if (num_in_ring > 1)
            ring++;
         num_in_ring = 0;
      }
      else
      {
         num_in_ring++;
         if (num_in_ring == 1)
            connection = *pASInputRings;
         if (num_in_ring > 1)
         {
            if (connection != NOT_USED)
            {
               NVRam_WriteByte( AutoScanInputStart + ring*2 + 0, connection );
               NVRam_WriteByte( AutoScanInputStart + ring*2 + 1, connection );
            }
         }
      }
   }
}

//******************************************************************************
//
// FUNCTION       : void AutoScanInput_GetNext(gmt_PHY_CH B_Channel,
//							gmt_UserDefConnNames *const pConnection)
//
// USAGE          : Called, when user presed "input select" key. Function is looking for default port in the
//							next ring. If CHECK_IS_CABLE_CONNECT is defined and default port is not connected to cable,
//							looks for next available port in the same ring.
//
// INPUT          : Channel, CurrentPort
//
// OUTPUT         : CurrentPort
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
void AutoScanInput_GetNext(gmt_PHY_CH B_Channel,	gmt_UserDefConnNames *const pConnection)
{
   BYTE num, ring, num_in_ring, found_ring, nvram_ring, next_ring;

   //search current connection's ring and next ring
   BYTE const far* pASInputRings = ASInputRings;
   BOOL is_ring0_saved = FALSE;
   BOOL B_isCableConnected;
   ring = num_in_ring = nvram_ring = 0;
   next_ring = found_ring = (BYTE)-1;
   for (num = AUTOSCANINPUT_RINGS_SIZE; num; num--, pASInputRings++)
   {
      if (*pASInputRings == NOT_USED)
      {
         ring++;
         if (num_in_ring > 1)
            nvram_ring++;
         num_in_ring = 0;
      }
      else
      {
         num_in_ring++;

         if ((ring==0) && (num_in_ring > 1))
            is_ring0_saved = TRUE;

         if (found_ring == (BYTE)-1)
         {
            if (*pASInputRings == *pConnection)
               found_ring = ring;
         }
         else
         {
            if ((next_ring == (BYTE)-1) && (found_ring != ring))
            {
               next_ring = ring; //next ring found
               *pConnection = *pASInputRings;
               B_isCableConnected = isCableConnected(*pConnection);
            }

            if ((next_ring==ring) && (num_in_ring > 1))
            {
               if (num_in_ring == 2)
               {	//Arbitrary
                  BYTE def_conn = NVRam_ReadByte(AutoScanInputStart + nvram_ring*2 + B_Channel);
                  if (isCableConnected(def_conn))
                  {
                     *pConnection = def_conn;
                     return;
                  }
                  if (B_isCableConnected)
                     return;
               }

               if (isCableConnected(*pASInputRings))
               {
                  *pConnection = *pASInputRings;
                  return;
               }
            }
         }
      }
   }

   if (next_ring == (BYTE)-1)
      *pConnection = (is_ring0_saved) ? (NVRam_ReadByte(AutoScanInputStart + B_Channel)) : 0;
}

//**********************************************************************************
#endif //AUTO_SCAN_INPUT_NVRAM

#endif //AUTO_SCAN_INPUT
/***********************************  END  ************************************/


