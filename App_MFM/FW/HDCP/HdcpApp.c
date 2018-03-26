/************************************************************************************************************

                                  COPYRIGHT (C) STMicroelectronics 2004.

	All rights reserved. This document contains proprietary and confidential information of the STMicro-
	electronics Group. This document is not to be copied in whole or part. STMicroelectronics assumes no
	responsibility for the consequences of use of such information nor for any infringement	of patents
	or other rights of third parties which may result from its use. No license is granted by implication
	or otherwise under any patent or patent rights of STMicroelectronics. STMicroelectronics products are
	not authorized for use as critical components in life support devices or systems without express writ-
	ten approval of STMicroelectronics.

File name 		:  HdcpApp.c
Author 			: 
Description		: 
Creation 		: 
*************************************************************************************************************/


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"
#if (FEATURE_DISPLAY_PORT == ENABLE) && (FEATURE_HDCP_REPEATER == ENABLE)
#include "HDCP\HdcpRepeaterLibExport\HdcpRepeaterBasicInclude.h"
#include "HDCP\HdcpRepeaterLibExport\HdcpRepeater.h"
#include "HDCP\HdcpApp.h"
#include "DP\DP1_2Libdevice\DPlibInterface.h"
#include "mem.h"

//******************************************************************************
//  D E B U G
//******************************************************************************
//#define DEBUG_HDCP_RPTR_MSG
#ifdef DEBUG_HDCP_RPTR_MSG
#define HdcpRptr_msg(x,y)        gm_Print(x,y)
#define HdcpRptr_msg_ii(x,y,z)   gm_Print(x,y,z)
#else
#define HdcpRptr_msg(x,y)
#define HdcpRptr_msg_ii(x,y,z)
#endif

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#if 1
#define NUM_HDCP_REPEATERS					1
#if (FEATURE_HDMI== ENABLE) && (DP_NUM_SST_ONLY_TX !=0) && (ENABLE_HDMIDVI_HDCP_REPEATER!=0)
#define RPTR1_NUM_UP_STRM_PORTS			DP_TOTAL_NUM_UPSTRM_PORTS + NUM_OF_HDMIDVI_INP
#else
#define RPTR1_NUM_UP_STRM_PORTS			DP_TOTAL_NUM_UPSTRM_PORTS // + Hdmi device
#endif
#define RPTR1_NUM_DWN_STRM_PORTS	      (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS + DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS + DP_NUM_SST_ONLY_TX)
#else
#define NUM_HDCP_REPEATERS					1
#define RPTR1_NUM_UP_STRM_PORTS			1
#define RPTR1_NUM_DWN_STRM_PORTS	      2
#endif
//******************************************************************************
//	G L O B A L   V A R I A B L E S
//******************************************************************************
HdcpRepeaterInstanceData_t      *CurrHdcpRep_ps;

HdcpRepeaterInstanceData_t      *HdcpRep_ps = NULL;

#if (FEATURE_HDCP_REPEATER == ENABLE)

HdcpRepeaterInstanceData_t      HdcpRptrData[NUM_HDCP_REPEATERS];

#if RPTR1_NUM_UP_STRM_PORTS
HdcpUpStreamDeviceInfo_t        Rptr1UpDeviceInfo[RPTR1_NUM_UP_STRM_PORTS];
#endif

#if RPTR1_NUM_DWN_STRM_PORTS
HdcpDownStreamDeviceInfo_t      Rptr1DwnDeviceInfo[RPTR1_NUM_DWN_STRM_PORTS];
#endif

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
extern void huge gm_Print(const far char *Bp_String, ...);
extern U32 EXT_FUNC gm_GetSystemTime(void);
extern BOOL FAR  DP_GetLogicalPortBksv (U8 *Bksv_p8);
//******************************************************************************
// FUNCTION:   BOOL FAR CreateRepeaterInstances(void)
// USAGE:      
// INPUT:      None
// OUTPUT:     None
// USED_REGS:  None
//******************************************************************************
BOOL FAR CreateRepeaterInstances(void)
{
   BYTE i=0;
	memset(HdcpRptrData, 0, sizeof(HdcpRptrData));
#if RPTR1_NUM_UP_STRM_PORTS
	memset(Rptr1UpDeviceInfo, 0, sizeof(Rptr1UpDeviceInfo));
#endif
#if RPTR1_NUM_DWN_STRM_PORTS
	memset(Rptr1DwnDeviceInfo, 0, sizeof(Rptr1DwnDeviceInfo));
#endif	

	HdcpRptrData[0].HdcpRepeaterIdentifier_8		= HdcpDpRepeater;
	HdcpRptrData[0].NumUpStreamPorts_4				= RPTR1_NUM_UP_STRM_PORTS;
	HdcpRptrData[0].NumDownStreamPorts_4			= RPTR1_NUM_DWN_STRM_PORTS;
#if RPTR1_NUM_UP_STRM_PORTS	
	HdcpRptrData[0].UpStreamDevices_ps				= Rptr1UpDeviceInfo;
#else
   HdcpRptrData[0].UpStreamDevices_ps           = NULL_PTR;
#endif

#if RPTR1_NUM_DWN_STRM_PORTS
	HdcpRptrData[0].DownStreamDevices_ps			= Rptr1DwnDeviceInfo;
#else
   HdcpRptrData[0].DownStreamDevices_ps         = NULL_PTR;
#endif	

   #if (DP_RL_NUM_UPSTRM_PORTS!=0)
	Rptr1UpDeviceInfo[i].DeviceIdentifier_8		    = MstRxPort;
	Rptr1UpDeviceInfo[i].HdcpRepeaterRxEntry_pf		= (HdcpRepeaterRxEntry_t)DpHdcpRptrRxEntry;
	i++;
	#endif

   #if (DP_NUM_SST_ONLY_RX!=0)
	Rptr1UpDeviceInfo[i].DeviceIdentifier_8		    = SSTLegacyRxPort;
	Rptr1UpDeviceInfo[i].HdcpRepeaterRxEntry_pf		= (HdcpRepeaterRxEntry_t)DpHdcpRptrRxEntry;
	i++;
   #endif

   #if (FEATURE_HDMI== ENABLE) && (NUM_OF_HDMIDVI_INP!=0) && (DP_NUM_SST_ONLY_TX !=0) && (ENABLE_HDMIDVI_HDCP_REPEATER!=0)
   {
      BYTE b_HdmiDviInNo,b_InputConnector;
      gmt_HDMI_DVI_DID b_DID;
      b_HdmiDviInNo = 0;
      b_InputConnector = 0;

      while ( (b_HdmiDviInNo < NUM_OF_HDMIDVI_INP) && (b_InputConnector < NUM_MASTER_LIST))
      {
         if (GetPortSignalType(b_InputConnector)  == SIG_DVI)
         {
            switch (b_InputConnector)
            {
               case ALL_BOARDS_DVI1:
                  Rptr1UpDeviceInfo[i].DeviceIdentifier_8          = Dvi1RxPort;
                  break;

               case ALL_BOARDS_DVI2:
                  Rptr1UpDeviceInfo[i].DeviceIdentifier_8          = Dvi2RxPort;
                  break;

               case ALL_BOARDS_HDMI:
                  Rptr1UpDeviceInfo[i].DeviceIdentifier_8          = Hdmi1RxPort;
                  break;

               case ALL_BOARDS_HDMI2:
                  Rptr1UpDeviceInfo[i].DeviceIdentifier_8          = Hdmi2RxPort;
                  break;

               case ALL_BOARDS_HDMI3:
                  Rptr1UpDeviceInfo[i].DeviceIdentifier_8          = Hdmi3RxPort;
                  break;
            }

            Rptr1UpDeviceInfo[i].HdcpRepeaterRxEntry_pf     = (HdcpRepeaterRxEntry_t)HdmiRxHdcpRptrRxEntry;
            b_HdmiDviInNo++;
            i++;
         }
         b_InputConnector++;
      }
   }
   #endif

   i=0;

   #if (DP_NUM_SST_ONLY_TX!=0)
	Rptr1DwnDeviceInfo[i].DeviceIdentifier_8		= iDpTxPort;
	Rptr1DwnDeviceInfo[i].HdcpRepeaterTxEntry_pf    = (HdcpRepeaterTxEntry_t)DpHdcpRptrTxEntry;
	i++;
   #endif
   
   #if (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS!=0)
	Rptr1DwnDeviceInfo[i].DeviceIdentifier_8		= MstTxPort;
	Rptr1DwnDeviceInfo[i].HdcpRepeaterTxEntry_pf    = (HdcpRepeaterTxEntry_t)DpHdcpRptrTxEntry;
	i++;
	#endif


   #if (DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS!=0) //If BKSV is not available dont add logical port to device info
	Rptr1DwnDeviceInfo[i].DeviceIdentifier_8		= LogicalTxPort;
	Rptr1DwnDeviceInfo[i].HdcpRepeaterTxEntry_pf    = (HdcpRepeaterTxEntry_t)DpHdcpRptrTxEntryLogicalPort;
	i++;
	#endif

	if(CreateHdcpRepeater(&HdcpRptrData[0])== FALSE)
		return FALSE;

#ifdef DEBUG_HDCP_RPTR_MSG
	gm_Print("Rptr Ins",0);

   #if RPTR1_NUM_UP_STRM_PORTS
	for(i=0;i<RPTR1_NUM_UP_STRM_PORTS;i++)
	   gm_Print("Rptr1UpDeviceInfo[%d]=0x%x",i,Rptr1UpDeviceInfo[i].DeviceIdentifier_8);
	#endif

	#if RPTR1_NUM_DWN_STRM_PORTS
   for(i=0;i<RPTR1_NUM_DWN_STRM_PORTS;i++)
      gm_Print("Rptr1DwnDeviceInfo[%d]=0x%x",i,Rptr1DwnDeviceInfo[i].DeviceIdentifier_8);
   #endif
#endif
   HdcpRepeater_PrintVersion();
   return TRUE;
}

#endif //#if (FEATURE_HDCP_REPEATER == ENABLE)

//******************************************************************************
// FUNCTION:   U32 FAR GetRepeaterId(U8 PortId_8)
// USAGE:      
// INPUT:      None
// OUTPUT:     None
// USED_REGS:  None
//******************************************************************************
U32 FAR GetRepeaterId(U8 PortId_8)
{
#if (FEATURE_HDCP_REPEATER == ENABLE)
	U8 Index_8;
	U8 Port_8;
	
	for(Index_8 = 0; Index_8 < NUM_HDCP_REPEATERS; Index_8++)
	{
		for(Port_8 = 0; Port_8 < HdcpRptrData[Index_8].NumDownStreamPorts_4; Port_8++)
		{
			if(HdcpRptrData[Index_8].DownStreamDevices_ps[Port_8].DeviceIdentifier_8 == PortId_8)
				return HdcpRptrData[Index_8].HdcpRepeaterIdentifier_8;
		}
		for(Port_8 = 0; Port_8 < HdcpRptrData[Index_8].NumUpStreamPorts_4; Port_8++)
		{
			if(HdcpRptrData[Index_8].UpStreamDevices_ps[Port_8].DeviceIdentifier_8 == PortId_8)
				return HdcpRptrData[Index_8].HdcpRepeaterIdentifier_8;
		}
	}
#else	
   UNUSED_PARAMETER(PortId_8);
#endif	
	return 0xFF;
}


//******************************************************************************
// FUNCTION:   U32 FAR HDCP_Ext_GetSystemTime (void)
// USAGE:      
// INPUT:      None
// OUTPUT:     None
// USED_REGS:  None
//******************************************************************************
U32 FAR HDCP_Ext_GetSystemTime (void)
{
	return gm_GetSystemTime();
}

#endif // (FEATURE_DISPLAY_PORT == ENABLE)


