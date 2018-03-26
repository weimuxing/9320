/*
	$Workfile:   CableStatus.c  $
	$Revision: 1.17 $
	$Date: 2013/01/18 05:48:08 $
*/
#define __CABLESTATUS_C__

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
//******************************************************************************

//==============================================================================
//
// MODULE:      CableStatus.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************
#if ((FEATURE_DISPLAY_PORT == ENABLE) || (FEATURE_HDMI == ENABLE))

//#define DEBUG_MSG
#ifdef DEBUG_MSG
#define msg(x)				gm_Print(x,0)
#define msg_i(x,y)			gm_Print(x,y)
#define msg_ii(x,y,z)		gm_Printf(x,y,z)
#else
#define msg(x)
#define msg_i(x,y)
#define msg_ii(x,y,z)
#endif

//******************************************************************************
//  E X T E R N S
//******************************************************************************


//******************************************************************************
//  G L O B AL
//******************************************************************************


//******************************************************************************
//  L O C A L S
//******************************************************************************
static gmt_CABLE_STATE b_CableState[AID_PHY_END];
static gmt_SRCPWR_STATE b_SrcPwrState[AID_PHY_END];
#ifdef DEBUG_MSG
static gmt_CABLE_STATE b_PreCableState[AID_PHY_END];
static gmt_SRCPWR_STATE b_PreSrcPwrState[AID_PHY_END];
#endif


//******************************************************************************
// FUNCTION       : void ComboPhyCableStateInit(void)
// USAGE          : Called from SystemInit for ComboPhy Cable State Init
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void ComboPhyCableStateInit(void)
{
   BYTE i;
   for (i=0;i<AID_PHY_END;i++)
   {

      b_CableState[i]=CAB_DISCONNECTED;
      b_SrcPwrState[i]=SRCPWR_OFF;

#ifdef DEBUG_MSG
      b_PreCableState[i]=CAB_DISCONNECTED;
      b_PreSrcPwrState[i]=SRCPWR_OFF;
#endif
   }
}


//******************************************************************************
// FUNCTION       : static BOOL IsComboPhyCableConnected(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Check Cable State for ComboPhy Input Port
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
static BOOL IsComboPhyCableConnected(gmt_COMBOPHY_AID b_PhyNo)
{
   CableInfo Info;
	Get_CableStatus(&Info);
	
#ifdef MUX_TEST_BOARD
	if(b_PhyNo == AID_PHY0)
	{		
		if(GetDpInputConnector(DID_DPRX0) == ALL_BOARDS_DP1)
			return Info.PHY1_CableStatus;
		else
			return Info.MiniDP_CableStatus;
	}	
#else		
	if(b_PhyNo == AID_PHY0)
		return Info.PHY1_CableStatus;
#endif		
        else if ((b_PhyNo == AID_PHY1) || (b_PhyNo == AID_PHY12))
      return Info.PHY2_CableStatus;
   else if (b_PhyNo == AID_PHY2)
      return Info.PHY3_CableStatus;
   else
      return FALSE;
}


//******************************************************************************
// FUNCTION       : gmt_CABLE_STATE GetComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Get Cable State for ComboPhy Input Port
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : gmt_CABLE_STATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
gmt_CABLE_STATE GetComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
{
   return b_CableState[b_PhyNo];
}

//******************************************************************************
// FUNCTION       : gmt_CABLE_STATE GetDpRxComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Get Cable State for DpRx ComboPhy Input Port 
//                : (To workaround unreliable cable status of DP source)
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : gmt_CABLE_STATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
#if (UNRELIABLE_DP_CAB_STS_WKAROUND !=0)
gmt_CABLE_STATE GetDpRxComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
{
   if(b_CableState[b_PhyNo]==CAB_DISCONNECTED)
   {
      if(GetComboPhySrcPwrState(b_PhyNo)==SRCPWR_ON)
         return CAB_CONNECTED;
      else if(GetComboPhySrcPwrState(b_PhyNo)==SRCPWR_DETECTING)
         return CAB_PLUGGING;
      else if(GetComboPhySrcPwrState(b_PhyNo)==SRCPWR_LOSTING) 
         return CAB_UNPLUGGING;
   }
   return b_CableState[b_PhyNo];
}
#endif   

//******************************************************************************
// FUNCTION       : gmt_CABLE_STATE UpdateComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Called Update ComboPhy Cable State
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : gmt_CABLE_STATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
gmt_CABLE_STATE UpdateComboPhyCableState(gmt_COMBOPHY_AID b_PhyNo)
{
   BYTE b_Cable;
   b_Cable=IsComboPhyCableConnected(b_PhyNo);

   switch (b_CableState[b_PhyNo])
   {
      case CAB_DISCONNECTED:
         if (b_Cable==TRUE)
            b_CableState[b_PhyNo]++;  //CAB_PLUGGING
         break;
      case CAB_PLUGGING:
         b_CableState[b_PhyNo]++;  //CAB_CONNECTED
#if (DEBUG_HDMI_PRINT_TIMING_DISPLAY_TIME==2)       
         if(b_PhyNo>=AID_PHY1) 
         {
           SetDebugDisplayTime();
         }
#endif         
#if (DEBUG_DP_PRINT_TIMING_DISPLAY_TIME==1)       
         if(b_PhyNo==AID_PHY0) 
         {
           SetDebugDisplayTime();
         }
#endif         

         break;
      case CAB_CONNECTED:
         if (b_Cable==FALSE)
            b_CableState[b_PhyNo]++;  //CAB_UNPLUGGING
         break;
      case CAB_UNPLUGGING:
         b_CableState[b_PhyNo]=CAB_DISCONNECTED;  //CAB_DISCONNECTED
         break;
   }

#ifdef DEBUG_MSG
	if(b_PreCableState[b_PhyNo]!=b_CableState[b_PhyNo])
	{
		b_PreCableState[b_PhyNo]=b_CableState[b_PhyNo];
		switch(b_CableState[b_PhyNo])
		{
		   case CAB_DISCONNECTED:
		      gm_Print("---Cphy(%d):CAB_DISCONNECTED",b_PhyNo);
		      break;
         case CAB_PLUGGING:
            gm_Print("---Cphy(%d):CAB_PLUGGING",b_PhyNo);
            break;
         case CAB_CONNECTED:
            gm_Print("---Cphy(%d):CAB_CONNECTED",b_PhyNo);
            break;
         case CAB_UNPLUGGING:
            gm_Print("---Cphy(%d):CAB_UNPLUGGING",b_PhyNo);
            break;
		}
	}
#endif	

	#ifdef ST_4K2K_93xx_BOARD 
		if(b_PhyNo == 2)		
			b_CableState[b_PhyNo] = CAB_CONNECTED;
	#endif
	
	return b_CableState[b_PhyNo];
}




//******************************************************************************
// FUNCTION       : static BOOL IsComboPhySrcPwrDetected(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Check Source Power State for ComboPhy Input Port
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
static BOOL IsComboPhySrcPwrDetected(gmt_COMBOPHY_AID b_PhyNo)
{
   CableInfo Info;
   Get_CableStatus(&Info);
	
#ifdef MUX_TEST_BOARD
	if(b_PhyNo == AID_PHY0)
	{		
		if(GetDpInputConnector(DID_DPRX0) == ALL_BOARDS_DP1)
			return Info.PHY1_SourcePowerStatus;
		else
			return Info.MiniDP_SourcePowerStatus;
	}	
#else			
	if(b_PhyNo == AID_PHY0)
		return Info.PHY1_SourcePowerStatus;
#endif	
	else if ((b_PhyNo == AID_PHY1) || (b_PhyNo == AID_PHY12))
      return Info.PHY2_SourcePowerStatus;
   else if (b_PhyNo == AID_PHY2)
      return Info.PHY3_SourcePowerStatus;
   else
      return FALSE;
}

//******************************************************************************
// FUNCTION       : gmt_SRCPWR_STATE GetComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Get ComboPhy Source Power State
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : gmt_SRCPWR_STATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
gmt_SRCPWR_STATE GetComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo)
{
   return b_SrcPwrState[b_PhyNo];
}



//******************************************************************************
// FUNCTION       : gmt_SRCPWR_STATE UpdateComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo)
// USAGE          : Called Update ComboPhy Source Power State
// INPUT          : gmt_COMBOPHY_AID
// OUTPUT         : gmt_SRCPWR_STATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
gmt_SRCPWR_STATE UpdateComboPhySrcPwrState(gmt_COMBOPHY_AID b_PhyNo)
{
   BYTE b_SrcPwr;
   b_SrcPwr=IsComboPhySrcPwrDetected(b_PhyNo);

   switch (b_SrcPwrState[b_PhyNo])
   {
      case SRCPWR_OFF:
         if (b_SrcPwr==TRUE)
            b_SrcPwrState[b_PhyNo]=SRCPWR_DETECTING;
         break;
      case SRCPWR_LOSTING:
         b_SrcPwrState[b_PhyNo]=SRCPWR_OFF;
         break;
      case SRCPWR_DETECTING:
         b_SrcPwrState[b_PhyNo]=SRCPWR_ON;
         break;
      case SRCPWR_ON:
         if (b_SrcPwr==FALSE)
            b_SrcPwrState[b_PhyNo]=SRCPWR_LOSTING;
         break;
   }

#ifdef DEBUG_MSG
	if(b_PreSrcPwrState[b_PhyNo]!=b_SrcPwrState[b_PhyNo])
	{
		b_PreSrcPwrState[b_PhyNo]=b_SrcPwrState[b_PhyNo];
		switch(b_SrcPwrState[b_PhyNo])
		{
		   case SRCPWR_DETECTING:
		      gm_Print("===Cphy(%d):SRCPWR_DETECTING",b_PhyNo);
		      break;
         case SRCPWR_OFF:
            gm_Print("===Cphy(%d):SRCPWR_OFF",b_PhyNo);
            break;
         case SRCPWR_ON:
            gm_Print("===Cphy(%d):SRCPWR_ON",b_PhyNo);
            break;
         case SRCPWR_LOSTING:
            gm_Print("===Cphy(%d):SRCPWR_LOSTING",b_PhyNo);
            break;
		}
	}
#endif	

	#ifdef ST_4K2K_93xx_BOARD  
		if(b_PhyNo == 2)
			b_SrcPwrState[b_PhyNo] = SRCPWR_ON;
	#endif

   return b_SrcPwrState[b_PhyNo];
}


#if (FEATURE_DISPLAY_PORT == ENABLE)
//******************************************************************************
// FUNCTION       : BOOL IsDpCableConnected(BYTE InputConnector)
// USAGE          : Check DP Cable is connected or not for Application Usage
// INPUT          : InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpCableConnected(BYTE InputConnector)
{
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);

   if (b_AID < AID_PHY_NONE)
   {
      if (b_CableState[b_AID] == CAB_CONNECTED)
         return TRUE;
   }
   return FALSE;
}



//******************************************************************************
// FUNCTION       : BOOL IsDpSrcPwrPresented(BYTE InputConnector)
// USAGE          : Check DP Source Power is detected or not for Application Usage
// INPUT          : InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpSrcPwrDetected(BYTE InputConnector)
{
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);

   if (b_AID < AID_PHY_NONE)
   {
      if (b_SrcPwrState[b_AID] == SRCPWR_ON)
         return TRUE;
   }
   return FALSE;
}
#endif


#if (FEATURE_HDMI == ENABLE)
//******************************************************************************
// FUNCTION       : BOOL IsHdmiDviCableConnected(BYTE InputConnector)
// USAGE          : Check Hdmi/Dvi Cable is connected or not for Application Usage
// INPUT          : InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsHdmiDviCableConnected(BYTE InputConnector)
{
   gmt_COMBOPHY_AID b_AID=gm_GetHdmiDviAnaPhyID(InputConnector);

   if (b_AID < AID_PHY_NONE)
   {
      if (b_CableState[b_AID] == CAB_CONNECTED)
         return TRUE;
   }
   return FALSE;
}
#endif

#endif //((FEATURE_DISPLAY_PORT == ENABLE) || (FEATURE_HDMI == ENABLE))


