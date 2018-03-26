/*
	$Workfile:   DpPwrStateCheck.c  $
	$Revision: 1.22 $
	$Date: 2013/01/21 10:39:26 $
*/
#define __DPPWRSTATECHECK_C__

//******************************************************************************
// COPYRIGHT (C) STMicroelectronics 2010 All rights reserved.
// This document contains proprietary and confidential information of the
// STMicroelectronics Group. This document is not to be copied in whole or part.
// STMicroelectronics assumes no responsibility for the consequences of use of
// such information nor for any infringement of patents or other rights of third
// parties which may result from its use. No license is granted by implication
// or otherwise under any patent or patent rights of STMicroelectronics.
// STMicroelectronics products are not authorized for use as critical components
// in life support devices or systems without express written approval of
// STMicroelectronics.
//******************************************************************************

//==============================================================================
//
// MODULE:      DPBrApp.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"
//******************************************************************************
//  D E F I N E S
//******************************************************************************
#if (FEATURE_DISPLAY_PORT == ENABLE)

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
#define DPRX_POWER_DOWN_MODE           0x02
#define DPRX_NORMAL_OPERATION_MODE     0x01
#define DPCD600CHECKTIME               500   //ms

static DpPwrCheck_t DpPwrChk[DID_DPRX_END];

#if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP !=0)
static BYTE LpmSigWkUp_Flag=FALSE;
#endif


void PrintDpPwrCheck(gmt_DPRX_DID b_DID)
{
   gmt_COMBOPHY_AID b_AID=(b_DID==DID_DPRX0)? AID_PHY0:AID_PHY2;
   BYTE bTimer=(b_DID==DID_DPRX0)? DP1_AUX_TRIGGER_10ms_TMR:DP2_AUX_TRIGGER_10ms_TMR;
   gm_Print("===PrintDpPwrCheck(DpRx%d)===",b_DID);
   gm_Print("DpcdSetPwrDwn  :%d",DpPwrChk[b_DID].b_SetPowerExCheck);
   gm_Print("SrcPwrOverride :%d",DpPwrChk[b_DID].b_SrcPwrOverrideFlag);
   gm_Print("SinkTesting    :%d",IsDpSinkInTesting(UserPrefCurrentInputMain));
   gm_Print("AuxTriggered   :%d",(gm_TimerCheck(bTimer)==TIMER_OK));

   if(DpPwrChk[b_DID].DpPwrState == DP_PWR_PHYON)
      gm_Print("PwrState       :DP_PWR_PHYON",0);
   else
      gm_Print("PwrState       :DP_PWR_PHYOFF",0);
   
   switch(GetComboPhySrcPwrState(b_AID))
   {
      case SRCPWR_OFF:
         gm_Print("SrcPower       :OFF",0);
         break;
      case SRCPWR_LOSTING:
         gm_Print("SrcPower       :LOSTING",0);
         break;
      case SRCPWR_DETECTING:
         gm_Print("SrcPower       :DETECTING",0);
         break;
      case SRCPWR_ON:
         gm_Print("SrcPower       :ON",0);
         break;
   }
   switch(GetComboPhyCableState(b_AID))
   {
      case CAB_DISCONNECTED:
         gm_Print("CableState_Raw :DISCONNECTED",0);
         break;
      case CAB_PLUGGING:
         gm_Print("CableState_Raw :PLUGGING",0);
         break;
      case CAB_CONNECTED:
         gm_Print("CableState_Raw :CONNECTED",0);
         break;
      case CAB_UNPLUGGING:
         gm_Print("CableState_Raw :UNPLUGGING",0);
         break;
   }
#if (UNRELIABLE_DP_CAB_STS_WKAROUND!=0)   
   switch(GetDpRxComboPhyCableState(b_AID))
   {
      case CAB_DISCONNECTED:
         gm_Print("CableState_WA  :DISCONNECTED",0);
         break;
      case CAB_PLUGGING:
         gm_Print("CableState_WA  :PLUGGING",0);
         break;
      case CAB_CONNECTED:
         gm_Print("CableState_WA  :CONNECTED",0);
         break;
      case CAB_UNPLUGGING:
         gm_Print("CableState_WA  :UNPLUGGING",0);
         break;
   }
#endif   
   
}



#if (UNRELIABLE_DP_CAB_STS_WKAROUND!=0)  
#define GetComboPhyCableState GetDpRxComboPhyCableState
#endif

//******************************************************************************
// FUNCTION       : void DpPwrCheckInit(void)
// USAGE          : Called from SystemInit for Dp Power State Check Init
// INPUT          : None
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void DpPwrCheckInit(void)
{
   BYTE i;
   for (i=0;i<DID_DPRX_END;i++)
   {
      DpPwrChk[i].dw_SetPowerExCheckTime=0;
      DpPwrChk[i].b_SetPowerExCheck=FALSE;
      DpPwrChk[i].b_SrcPwrOverrideFlag=FALSE;
      DpPwrChk[i].DpPwrState=DP_PWR_PHYON;
      DpPwrChk[i].PreDpPwrState=DP_PWR_UNKNOWN;
      #if (HPD_ON_PHYON_NOTIFICATION !=0)
      DpPwrChk[i].b_PhyOnNotify=FALSE;
      #if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP !=0)
      if(LpmSigWkUp_Flag==TRUE)
      {
         DpPwrChk[i].b_LpmWkUpNotify=TRUE;
         LpmSigWkUp_Flag=FALSE;
      }
      else
      {
         DpPwrChk[i].b_LpmWkUpNotify=FALSE;
      }
      #endif
      
      #endif
   }
}


//******************************************************************************
// FUNCTION       : ResetDPCD600(BYTE InputConnector)
// USAGE          : Called for Reset DPCD600 back to 01 for normal operation
// INPUT          : BYTE InputConnector
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void ResetDPCD600(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   BYTE far *W_Ptr;
   if (b_DID== DID_DPRX0)
   {
      W_Ptr=LINEAR_TO_FPTR_B(0x1F600);
   }
   else if (b_DID== DID_DPRX1)
   {
      W_Ptr=LINEAR_TO_FPTR_B(0x1B600);
   }
   else
   {
      msg_i("### IsDpcdSetPowerReallyDown- Failed (%d)",InputConnector);
      return;  //Incorrect return!
   }

   msg_i("ResetDPCD600(%d)", InputConnector);
   
   W_Ptr[0] = DPRX_NORMAL_OPERATION_MODE;

}


//******************************************************************************
// FUNCTION       : BOOL IsDpcdSetPowerReallyDown(BYTE InputConnector)
// USAGE          : Check DP SetPower (DPCD600) is really equal 2 or not
// INPUT          : BYTE InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpcdSetPowerReallyDown(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   BYTE far *W_Ptr;
   if (b_DID== DID_DPRX0)
   {
      W_Ptr=LINEAR_TO_FPTR_B(0x1F600);
   }
   else if (b_DID== DID_DPRX1)
   {
      W_Ptr=LINEAR_TO_FPTR_B(0x1B600);
   }
   else
   {
      msg_i("### IsDpcdSetPowerReallyDown- Failed (%d)",InputConnector);
      return FALSE;  //Incorrect return!
   }

   if (W_Ptr[0] != DPRX_POWER_DOWN_MODE)
   {
      DpPwrChk[b_DID].b_SetPowerExCheck=FALSE;
      DpPwrChk[b_DID].dw_SetPowerExCheckTime=0;
      return FALSE;
   }
   else
   {
      if (DpPwrChk[b_DID].b_SetPowerExCheck==TRUE)
         return TRUE;

      if (DpPwrChk[b_DID].dw_SetPowerExCheckTime)
      {
         if ((gm_GetSystemTime()-DpPwrChk[b_DID].dw_SetPowerExCheckTime)>DPCD600CHECKTIME)
         {
            DpPwrChk[b_DID].b_SetPowerExCheck=TRUE;
            return TRUE;
         }
      }
      else
      {
         DpPwrChk[b_DID].dw_SetPowerExCheckTime=gm_GetSystemTime();
      }
      return FALSE;
   }
}


//******************************************************************************
// FUNCTION       : BOOL IsDpSinkInTesting(BYTE InputConnector)
// USAGE          : Check DP is during DPCP Sink Test
// INPUT          : BYTE InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpSinkInTesting(BYTE InputConnector)
{
   UNUSED_PARAMETER(InputConnector);
   if (gm_TimerCheck(DPCD_SINK_TEST_1s_TMR) == TIMER_OK)
      return TRUE;
   return FALSE;
}




//******************************************************************************
// FUNCTION       : BOOL IsDpSrcPwrOverWritten(BYTE InputConnector)
// USAGE          : Indicate DP SrcPwrDetection is not working with
//                : the attached DP Transmitter
// INPUT          : BYTE InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpSrcPwrOverWritten(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   return DpPwrChk[b_DID].b_SrcPwrOverrideFlag;
}

#if 0
//******************************************************************************
// FUNCTION       : DP_PWRSTATE GetDpRxPwrState(BYTE InputConnector)
// USAGE          : Get DPRX Port Power State
// INPUT          : BYTE InputConnector
// OUTPUT         : DP_PWRSTATE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
DP_PWRSTATE GetDpRxPwrState(BYTE InputConnector)
{
   return DpPwrChk[gm_GetDpDigitalID(InputConnector)].DpPwrState;
}


//******************************************************************************
// FUNCTION       : void SetDpRxPwrState(BYTE InputConnector, DP_PWRSTATE Sts)
// USAGE          : Set DPRX Port Power State
// INPUT          : BYTE InputConnector , DP_PWRSTATE Sts
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void SetDpRxPwrState(BYTE InputConnector, DP_PWRSTATE Sts)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);

   PreDpPwrState[b_DID]=Sts;	//0xff;
   DpPwrState[b_DID]=Sts;
}
#endif

#if (HPD_ON_PHYON_NOTIFICATION != 0)
#if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP !=0)
void DpRx_SendLpmWakeUp_Notice(void)
{
   LpmSigWkUp_Flag=TRUE;
}
#endif

void SetPhyOnNotification(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   if(b_DID == DID_DPRX0)
   {
      gm_TimerStart(RX0_PHYON_NOTIFY_10ms_TMR,PHYON_NOTIFY_Time_x10ms);
   }
   else if(b_DID == DID_DPRX1)
   {
      gm_TimerStart(RX1_PHYON_NOTIFY_10ms_TMR,PHYON_NOTIFY_Time_x10ms);
   }
   DpPwrChk[b_DID].b_PhyOnNotify=TRUE;
}

void ClearPhyOnNotification(BYTE InputConnector)
{
   DpPwrChk[gm_GetDpDigitalID(InputConnector)].b_PhyOnNotify=FALSE;
}

void ActPhyOnNotification(BYTE InputConnector)
{
    gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
    BYTE bTimer=(b_DID==DID_DPRX0)? RX0_PHYON_NOTIFY_10ms_TMR:RX1_PHYON_NOTIFY_10ms_TMR;

    if(DpPwrChk[b_DID].b_PhyOnNotify==TRUE)
    {
      if(DpRxIsLT_Success(InputConnector)==TRUE)
      {
         DpPwrChk[b_DID].b_PhyOnNotify=FALSE;
         #if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP !=0)
         DpPwrChk[b_DID].b_LpmWkUpNotify=FALSE;
         #endif
         msg_i("***LT OK(%d)",InputConnector);
         return;
      }
      
      if(gm_TimerCheck(bTimer)!=TIMER_OK)
      {
         if(IsDpAuxTriggered(InputConnector)!=TRUE) 
         {
            BYTE bHpdTimer=(b_DID==DID_DPRX0)? RX0_HPD_OVERRIDED_10ms_TMR:RX1_HPD_OVERRIDED_10ms_TMR;
         #if (HPD_ON_PHYON_NOTIFICATION_ONLY_WAKEUP !=0)
            if(DpPwrChk[b_DID].b_LpmWkUpNotify==TRUE)
            {
               DpPwrChk[b_DID].b_LpmWkUpNotify=FALSE;
            }
            else
            {
               msg_i("@@@Ignore_Hpd_PhyOnNotify(%d)",InputConnector);
               DpPwrChk[b_DID].b_PhyOnNotify=FALSE;
               return;
            }
         #endif            
            ForceDpHpd2Low(b_DID,TRUE);
            SetForceHpdLowFlag(b_DID, TRUE);
            gm_TimerStart(bHpdTimer,HPD_OVERRIDED_PHY_ACT_TIMEx10ms);
            DpPwrChk[b_DID].b_PhyOnNotify=FALSE;
            msg_i("***Hpd_PhyOnNotify(%d)",InputConnector);
         }
      }
    }
}
#endif
//******************************************************************************
// FUNCTION       : BOOL DpRxPwrStateCheck(BYTE InputConnector)
// USAGE          : DP Extra Status Check to determine whether to change DP Power
//                  State or not.
// INPUT          : BYTE InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL DpRxPwrStateCheck(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);
#if (SRCPWR_OFF_DEBOUNCE_CHECK!=0)
   BYTE bTimer=(b_AID==AID_PHY0)? DP1_SRCPWR_OFF_DEBOUNCE_10ms_TMR:DP2_SRCPWR_OFF_DEBOUNCE_10ms_TMR;
#endif
   UpdateComboPhySrcPwrState(b_AID);
   UpdateComboPhyCableState(b_AID);

   if (GetComboPhyCableState(b_AID)==CAB_UNPLUGGING)
   {
      ResetDPCD600(InputConnector);
   }


#if (SRCPWR_OFF_DEBOUNCE_CHECK!=0)
   if(GetComboPhySrcPwrState(b_AID)==SRCPWR_LOSTING)
   {
      gm_TimerStart(bTimer, SRCPWR_OFF_DEBOUNCE_TIMEx10ms);
   }
#endif

   if(IsDpSinkInTesting(InputConnector)==TRUE)
   {
      DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYON;
   }
#if (IGNORE_SLEEP_IF_DPCD600_NOT_2==0)
   else if(IsDpcdSetPowerReallyDown(InputConnector)==TRUE)
   {
      DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYOFF;
   }
#endif
   else if(IsDpAuxTriggered(InputConnector)==TRUE)
   {
      DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYON;
   }
   else if (GetComboPhyCableState(b_AID)==CAB_DISCONNECTED)
   {
      DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYOFF;
   }
   else if ((GetComboPhySrcPwrState(b_AID)==SRCPWR_OFF) &&
            (IsDpSrcPwrOverWritten(InputConnector)==FALSE))
   {
      #if (SRCPWR_OFF_DEBOUNCE_CHECK!=0)
      if(gm_TimerCheck(bTimer)==TIMER_OK) //Running
         DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYON;
      else
      #endif
         DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYOFF;
   }
   else
   {
      DpPwrChk[b_DID].DpPwrState=DP_PWR_PHYON;
   }

   if (DpPwrChk[b_DID].PreDpPwrState!=DpPwrChk[b_DID].DpPwrState)
   {
      DpPwrChk[b_DID].PreDpPwrState=DpPwrChk[b_DID].DpPwrState;
      if (DpPwrChk[b_DID].DpPwrState==DP_PWR_PHYON)
      {
         #if (HPD_ON_PHYON_NOTIFICATION!=0)
         SetPhyOnNotification(InputConnector);
         #endif
         msg_i("DP(%d)_PWR_PHYON",b_DID);
      }
      else
      {
         #if (HPD_ON_PHYON_NOTIFICATION!=0)
         ClearPhyOnNotification(InputConnector);
         #endif
         msg_i("DP(%d)_PWR_PHYOFF",b_DID);
      }
      return TRUE;
   }

   #if (HPD_ON_PHYON_NOTIFICATION!=0)
   if (DpPwrChk[b_DID].DpPwrState==DP_PWR_PHYON)
      ActPhyOnNotification(InputConnector);
   #endif
   return FALSE;
}

#ifdef INACTIVE_DEVICE_POWER_SAVING

#define CPHY_L0_CTRL_0                                 0x0010 //33552 (CPHY_PHYA)
#define CPHY_L1_CTRL_0                                 0x001C //33564 (CPHY_PHYA)
#define CPHY_L2_CTRL_0                                 0x0028 //33576 (CPHY_PHYA)
#define CPHY_L3_CTRL_0                                 0x0034 //33588 (CPHY_PHYA)
//******************************************************************************
// FUNCTION       : void DpRxPowrUpDevice(BYTE InputConnector)
// USAGE          : Power up DPRX port
// INPUT          : BYTE InputConnector
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void DpRxPowrUpDevice(BYTE InputConnector)
{
   WORD BaseAddr;
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);
   if(b_AID == AID_PHY0)
      BaseAddr=CPHY0_AUX_PHY_CTRL;
   else if (b_AID == AID_PHY2)
      BaseAddr=CPHY2_AUX_PHY_CTRL;            
   else
      return;
   
   //Power down PHY 
   gm_ClearRegBitsDWord(BaseAddr + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
   gm_ClearRegBitsDWord(BaseAddr + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
   gm_ClearRegBitsDWord(BaseAddr + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
   gm_ClearRegBitsDWord(BaseAddr + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);
}

//******************************************************************************
// FUNCTION       : void DpRxPowrDownDevice(BYTE InputConnector)
// USAGE          : Power Down DPRX port
// INPUT          : BYTE InputConnector
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void DpRxPowrDownDevice(BYTE InputConnector)
{
   WORD BaseAddr;
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);
   if(b_AID == AID_PHY0)
      BaseAddr=CPHY0_AUX_PHY_CTRL;
   else if (b_AID == AID_PHY2)
      BaseAddr=CPHY2_AUX_PHY_CTRL;            
   else
      return;
 
   //Power down PHY 
   gm_SetRegBitsDWord(BaseAddr + CPHY_L0_CTRL_0 ,CPHY0_L0_IDDQEN);
   gm_SetRegBitsDWord(BaseAddr + CPHY_L1_CTRL_0 ,CPHY0_L1_IDDQEN);
   gm_SetRegBitsDWord(BaseAddr + CPHY_L2_CTRL_0 ,CPHY0_L2_IDDQEN);
   gm_SetRegBitsDWord(BaseAddr + CPHY_L3_CTRL_0 ,CPHY0_L3_IDDQEN);
}


//******************************************************************************
// FUNCTION       : void AdjustDpPwrLevel(BYTE InputConnector)
// USAGE          : Adjust (Turn on or off)DPRx Port Power
// INPUT          : BYTE InputConnector
// OUTPUT         : None
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
void AdjustDpPwrLevel(BYTE InputConnector)
{
   gmt_DPRX_DID b_DID=gm_GetDpDigitalID(InputConnector);
   //gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);
   if (DpPwrChk[b_DID].DpPwrState==DP_PWR_PHYON)
   {
      DpRxPowrUpDevice(InputConnector);
   }
   else
   {	
      DpRxPowrDownDevice(InputConnector);
   }
}
#endif // defined(INACTIVE_DEVICE_POWER_SAVING)

//******************************************************************************
// FUNCTION       : BOOL IsDpAuxTriggered(BYTE InputConnector)
// USAGE          : Check DP Transmitter has issued Aux Command or not
// INPUT          : BYTE InputConnector
// OUTPUT         : TRUE/FALSE
// GLOBALS        : None
// USED_REGS      : None
// PRE_CONDITION  :
// POST_CONDITION :
//******************************************************************************
BOOL IsDpAuxTriggered(BYTE InputConnector)
{
   gmt_COMBOPHY_AID b_AID=gm_GetDpAnaPhyID(InputConnector);
   if(b_AID == AID_PHY0)   
   {
      if(gm_TimerCheck(DP1_AUX_TRIGGER_10ms_TMR)==TIMER_OK)
         return TRUE;
   }
   else if(b_AID == AID_PHY2)   
   {
      if(gm_TimerCheck(DP2_AUX_TRIGGER_10ms_TMR)==TIMER_OK)
         return TRUE;
   }
   return FALSE;
}

void SetDp1AuxTrigger(void)
{
   gm_TimerStart(DP1_AUX_TRIGGER_10ms_TMR,AUX_TRIGGER_NOTIFICATION_Timex10ms);
   if(GetComboPhySrcPwrState(AID_PHY0)==SRCPWR_OFF)
      DpPwrChk[DID_DPRX0].b_SrcPwrOverrideFlag=TRUE;
}


void SetDp2AuxTrigger(void)
{
   gm_TimerStart(DP2_AUX_TRIGGER_10ms_TMR,AUX_TRIGGER_NOTIFICATION_Timex10ms);
   if(GetComboPhySrcPwrState(AID_PHY2)==SRCPWR_OFF)
      DpPwrChk[DID_DPRX1].b_SrcPwrOverrideFlag=TRUE;
}


BOOL DpRxSleepModeBlocker(BYTE InputConnector)
{

#if (INGORE_SLEEP_PHY_AUTOMATION !=0)
   #if DP_NUM_SST_ONLY_TX != 0
      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_FE)
      #endif
      {
         if(DP_IsTxActive(iDpTxPort))
         {
             if(DPTX_IsPhyAutomationInProgress(iDpTxPort)!= FALSE)
             {
                 msg_i("DpTxSMBlocker(iDpTxPort): Phy Automation",0);
                 return TRUE;
             }
         }
      }
   #endif
   
   #if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
   if(DP_IsTxActive(MstTxPort))
   {
      if(DPTX_IsPhyAutomationInProgress(MstTxPort)!= FALSE)
      {
          msg_i("DpTxSMBlocker(MstTxPort): Phy Automation",0);
          return TRUE;
      }
   }
   #endif
#endif


#if (IGNORE_SLEEP_IF_SRCPWR_IS_ON !=0)
   if(GetComboPhySrcPwrState(gm_GetDpAnaPhyID(InputConnector))==SRCPWR_ON)   
   {
      msg_i("DpRxSMBlocker(%d): SrcPwrOn",InputConnector);
      return TRUE;
   }
#endif
      
#if (IGNORE_SLEEP_IF_DPCD600_NOT_2 !=0)
//   if(DpPwrChk[gm_GetDpDigitalID(InputConnector)].DpPwrState ==DP_PWR_PHYON)
   if((IsDpcdSetPowerReallyDown(InputConnector)!=TRUE) && (GetComboPhySrcPwrState(gm_GetDpAnaPhyID(InputConnector))==SRCPWR_ON))
   {
      msg_i("DpRxSMBlocker(%d): DPCD600_NOT_2",InputConnector);
      return TRUE;
   }
#endif


#if(ENABLE_DPCD_SINK_TEST_SLEEP_DELAY != 0)
   if(IsDpSinkInTesting(InputConnector))
   {
      msg_i("DpRxSMBlocker(%d): SinkInTesting",InputConnector);
      return TRUE;
   }
#endif


#if (IGNORE_SLEEP_DPRX_IS_STABLE !=0)
   if(DpRxIsStableState(InputConnector))
   {
      msg_i("DpRxSMBlocker(%d): StableSts",InputConnector);
      return TRUE;
   }
#endif


   if (IsDpAuxTriggered(InputConnector))
   {
      msg_i("DpRxSMBlocker(%d): AuxTriggered",InputConnector);
      return TRUE;
   }

   return FALSE;
}


#endif

