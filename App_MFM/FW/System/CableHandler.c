/*
	$Workfile:   Cablehandler.c  $
	$Revision: 1.16 $
	$Date: 2013/04/03 03:18:41 $
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
//
//		---
//
//		MODULE:		ADCKeypad.c
//
//		USAGE:		ADC Keypad related functions
//
//******************************************************************************
//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "system\all.h"
#include <math.h>
//#include <embedded.h>

// #define _Dedug

BYTE LPM_CableStatus[8];

BOOL GetCableStatus_PHY(BYTE B_AID);
BOOL GetPowerSource_PHY(BYTE B_AID);

extern void UpdateCableStatus(CableInfo * CableUpdate);
// 20100802, Cables Status handler
void CableStatusHandler(void)
{
   if (!gm_MessageReceived(gmd_MESSAGE_CableStatus, &LPM_CableStatus, 8))
      return;
}

void RequestUpdateCableStatus(void)
{
	WORD command = 1;	// 1: request update cable status

	static DWORD DW_CheckTime = 0;
	if(abs((WORD)(gm_GetSystemTime() - DW_CheckTime))< 300)
	return;
	
	DW_CheckTime = gm_GetSystemTime();	 

	gm_MessageSend(gmd_MESSAGE_CHECK_STATUS, &command, sizeof(command));
}

//120912 Edward follow Neil's comment , and cable update request only for HDMI / DVI from ADC
BOOL IS_RequestCableStatusUpdate(void)
{
	
	if(Is_PIP_ON())
	{
		if(IsMainHDMIInput()||IsMainDVID1Input()||IsMainVGAInput()||
		   IsPIPHDMIInput()||IsPIPDVID1Input()||IsPIPVGAInput())
			return TRUE;
	}
	else
	{
		if(IsMainHDMIInput()||IsMainDVID1Input()||IsMainVGAInput())
			return TRUE;
	}
	return FALSE;
}

void Get_CableStatus(CableInfo * CableUpdate)
{

   if(IS_RequestCableStatusUpdate())
   	RequestUpdateCableStatus();	

   UpdateCableStatus(CableUpdate);
}

BOOL isCableConnected(gmt_UserDefConnNames B_InputConnector)
{
   //gm_Delay10ms(1);
   if ((GetPortConnType(B_InputConnector)==HDMI)||(GetPortConnType(B_InputConnector)==DVI))
   {
      //gm_Print("HDMI PHY ID = %d", ((gmt_HdmiDviProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
      return GetCableStatus_PHY(((gmt_HdmiDviProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
   }
   else if (GetPortConnType(B_InputConnector)==CT_DISPLAY_PORT)
   {
      /*ChengLin 20130403: Correct DP1 and DP2(Mini DP) Cable Status*/
#ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_R)
   {
      CableInfo CableUpdate;
      BOOL PHY1_CableStatus = FALSE;
      Get_CableStatus(&CableUpdate);

      if(B_InputConnector == ALL_BOARDS_DP1)
         PHY1_CableStatus = CableUpdate.PHY1_CableStatus;
      else if(B_InputConnector == ALL_BOARDS_DP3)
         PHY1_CableStatus = CableUpdate.MiniDP_CableStatus;
      return PHY1_CableStatus;
   }
   else
   {
      //gm_Print("DP PHY ID = %d", ((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
      return GetCableStatus_PHY(((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
   }
#else   
		CableInfo CableUpdate;
		Get_CableStatus(&CableUpdate);
      //gm_Print("DP PHY ID = %d", ((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
      //return GetCableStatus_PHY(((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
		if(B_InputConnector == DP1)
			return CableUpdate.PHY1_CableStatus;
		#ifdef MUX_TEST_BOARD
		else if(B_InputConnector == DP3)
			return CableUpdate.MiniDP_CableStatus;
		#endif
#endif
   }
   else if (GetPortConnType(B_InputConnector)==VGA)
   {
      //gm_Print("VGA Cable ................ %d", GetCableStatus_PHY(PHY_VGA));
      return GetCableStatus_PHY(PHY_VGA);
   }
   //else
      return TRUE;
}

BOOL isSourcePowerConnected(gmt_UserDefConnNames B_InputConnector)
{
   //gm_Delay10ms(1);
   if ((GetPortConnType(B_InputConnector)==HDMI)||(GetPortConnType(B_InputConnector)==DVI))
   {
      //gm_Print("HDMI PHY ID = %d", ((gmt_HdmiDviProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
      return GetPowerSource_PHY(((gmt_HdmiDviProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
   }
   else if (GetPortConnType(B_InputConnector)==CT_DISPLAY_PORT)
   {
    /*ChengLin 20130403: Correct DP1 and DP2(Mini DP) Source Power Status*/
#ifdef ST_4K2K_93xx_BOARD
     if(GetChipID_4K2K()==CHIPID_4K2K_R)
     {
        CableInfo CableUpdate;
        BOOL PHY1_SrcPwrStatus = FALSE;        
        Get_CableStatus(&CableUpdate);
      
        if(B_InputConnector == ALL_BOARDS_DP1)
            PHY1_SrcPwrStatus = CableUpdate.PHY1_SourcePowerStatus;
        else if(B_InputConnector == ALL_BOARDS_DP3)
            PHY1_SrcPwrStatus = CableUpdate.MiniDP_SourcePowerStatus;
        return PHY1_SrcPwrStatus;
     }
     else
     {
         //gm_Print("DP PHY ID = %d", ((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
         return GetPowerSource_PHY(((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
     }
#else
      //gm_Print("DP PHY ID = %d", ((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
      #ifdef DELL_U2410F
      {
	  		CableInfo CableUpdate;
			Get_CableStatus(&CableUpdate);

			if(B_InputConnector == DP1)
				return CableUpdate.PHY1_SourcePowerStatus;
			#ifdef MUX_TEST_BOARD
			else if(B_InputConnector == DP3)
				return CableUpdate.MiniDP_SourcePowerStatus;
			#endif
                         return TRUE;
      }
	  #else
      return GetPowerSource_PHY(((gmt_DpProperties far *)(GetPortPropertyTable(B_InputConnector)))->B_AID);
	  #endif
#endif
   }
   else if (GetPortConnType(B_InputConnector)==VGA)
   {
      //gm_Print("VGA Cable ................ %d", GetCableStatus_PHY(PHY_VGA));
      return GetPowerSource_PHY(PHY_VGA);
   }
   else
      return TRUE;
}

BOOL GetCableStatus_PHY(BYTE B_AID)
{

   CableInfo CableUpdate;
   Get_CableStatus(&CableUpdate);


   if (B_AID == PHY1)
      return CableUpdate.PHY1_CableStatus;
   else if (B_AID == PHY2)
      return CableUpdate.PHY2_CableStatus;
   else if (B_AID == PHY3)
      return CableUpdate.PHY3_CableStatus;
   else if (B_AID == PHY12)
      return CableUpdate.PHY2_CableStatus;
   else if (B_AID == PHY_VGA)
      return CableUpdate.VGA_CableStatus;
   else
      return TRUE;
}

BOOL GetPowerSource_PHY(BYTE B_AID)
{
   CableInfo CableUpdate;
   Get_CableStatus(&CableUpdate);

   if (B_AID == PHY1)
      return CableUpdate.PHY1_SourcePowerStatus;
   else if (B_AID == PHY2)
      return CableUpdate.PHY2_SourcePowerStatus;
   else if (B_AID == PHY3)
      return CableUpdate.PHY3_SourcePowerStatus;
   else if (B_AID == PHY12)
      return CableUpdate.PHY2_SourcePowerStatus;
   else if (B_AID == PHY_VGA)
      return CableUpdate.VGA_SourcePowerStatus;
   else
      return TRUE;
}

//*********************************  END  **************************************

