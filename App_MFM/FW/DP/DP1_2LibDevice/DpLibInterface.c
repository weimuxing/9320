/************************************************************************************************************

                                  COPYRIGHT (C) STMicroelectronics 2004.

	All rights reserved. This document contains proprietary and confidential information of the STMicro-
	electronics Group. This document is not to be copied in whole or part. STMicroelectronics assumes no
	responsibility for the consequences of use of such information nor for any infringement	of patents
	or other rights of third parties which may result from its use. No license is granted by implication
	or otherwise under any patent or patent rights of STMicroelectronics. STMicroelectronics products are
	not authorized for use as critical components in life support devices or systems without express writ-
	ten approval of STMicroelectronics.

File name 		:
Author 			:
Description		:
Creation 		:
*************************************************************************************************************/

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#define DISABLE 0
#define ENABLE 1
#include "StdLibInc\gm_ChipFeature.h"

#if (FEATURE_DISPLAY_PORT == ENABLE)
#include <mem.h>
#include "DP\DP1_2LibDevice\DpLibInterface.h"
#include "DP\DP1_2LibExport\DpRxLib.h"
#include "DP\DP1_2LibExport\DP12_Rx.h"
#include "DP\DP1_2LibExport\DpTxLib.h"
#include "DP\DP1_2LibExport\DP12_Tx.h"
#include "DP\DP1_2LibExport\DPbridgetop.h"
#include "DP\DP1_2LibExport\regaccess.h"
#include "DP\DP1_2LibExport\DP12_Bu.h"
#include "DP\DP1_2LibExport\DP12_Port.h"
#include "StdLibInc\gm_Register.h"
#include "StdLibInc\gm_global.h"



//******************************************************************************
//	V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//	F U N C T I O N   E X T E R N S
//******************************************************************************

#ifdef ST_4K2K_93xx_BOARD
typedef enum
{
   CHIPID_4K2K_NONE,
   CHIPID_4K2K_L,
   CHIPID_4K2K_FE,
   CHIPID_4K2K_R   
}gmt_4k2k_ChipID;
#endif

extern void far gm_Delay10ms(U16 value);
extern void AddToString (BYTE * FV, BYTE number);
extern void DP_EXT_FUNC		gm_Print(const far char *String, U16 W_Value);
//extern void DP_EXT_FUNC		SBM_Isr(void);
//extern void DP_EXT_FUNC		HPD_HpdSetHigh(void);
//extern void DP_EXT_FUNC		HPD_HpdSetLow(void);
#if (PCLK_VALUE_FROM_AVDDS!=0)
extern DWORD DpRx_UpdatePixelClkExt(BYTE PortId);
#endif
extern void SetDp1AuxTrigger(void);
extern void SetDp2AuxTrigger(void);
extern void DpMstRxPowerManagementBeforeLT(void);
extern BYTE GetChipID_4K2K(void);

//******************************************************************************
//	L O C A L   C O N S T A N T   D E F I N E S
//******************************************************************************
//#define _DEBUG_MSG
#ifdef _DEBUG_MSG
#define msg(x,y)        gm_Print(x,y)
#define msg_ii(x,y,z)   gm_Print(x,y,z)
#else
#define msg(x,y)
#define msg_ii(x,y,z)
#endif
//******************************************************************************
//	L O C A L   M A C R O   D E F I N E S
//******************************************************************************
#define SST_ONLY_PORT_NUMBER		0xFF
#define SST_ONLY_HARDWARE_MAP		0xFF


//******************************************************************************
//	G L O B A L   V A R I A B L E S
//******************************************************************************

#if (DP_NUM_SST_ONLY_TX != 0 ) || (DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0)
DpTxFeSstMstCommonData_t	DpTxFeSstMstCommonData_s[NUM_OF_TX_FE];
#endif

#if (DP_RL_NUM_UPSTRM_PORTS != 0)
DpRxFeSstMstCommonData_t	DpRxFeSstMstCommonData_s[NUM_OF_RX_FE];
#endif

#if DP_NUM_SST_ONLY_TX != 0
DpTxSstFeInstanceData_t 	DPTX_SstFeData[DP_NUM_SST_ONLY_TX];
DpTxSstBeInstanceData_t 	DPTX_SstBeData[DP_NUM_SST_ONLY_TX];
DPTX_SstOnly_t				DPTX_SstOnlyPortInstanceData[DP_NUM_SST_ONLY_TX];
#endif

#if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
DpTxMstBePortSpecificInstanceData_t	DPTX_MstPortSpecificBeData[DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS];
DpTxMstFeInstanceData_t 			DPTX_MstFeData[DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS];
DpTxMstBeInstanceData_t 			DPTX_MstBeData[DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS];
DPTX_MstCapable_t				    DPTX_MstPortInstanceData[DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS];
#endif

#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
DpTxMstBePortSpecificInstanceData_t	DPTX_LogPortSpecificBeData[DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS];
DpTxMstBeInstanceData_t 			DPTX_LogPortBeData[DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS];
DpRxSstBeInstanceData_t				DPRX_LogPortSstBeData[DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS];
DPTX_DigitalTransmitter_t			DPTX_LogPortInstanceData[DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS];
#endif

#if DP_NUM_SST_ONLY_RX != 0
DpRxSstFeInstanceData_t 	DPRX_SstFeData[DP_NUM_SST_ONLY_RX];
DpRxSstBeInstanceData_t 	DPRX_SstBeData[DP_NUM_SST_ONLY_RX];
DPRX_SstOnly_t				DPRX_SstOnlyPortInstanceData[DP_NUM_SST_ONLY_RX];
#endif

#if DP_RL_NUM_UPSTRM_PORTS != 0
DpRxMstFeInstanceData_t 	DPRX_MstFeData[DP_RL_NUM_UPSTRM_PORTS];
DpRxMstBeInstanceData_t 	DPRX_MstBeData[DP_RL_NUM_UPSTRM_PORTS];
DPRX_MstCapable_t			DPRX_MstPortInstanceData[DP_RL_NUM_UPSTRM_PORTS];
// Can't have routing logic (branching unit)
// without upstream MST capable ports.
DPRL_Params_t		DPRL_Upkt_s;
#endif

DPRL_Params_t *		DPRL_RlParam_ps = NULL;

PortStructure_t *	DP_PortList;

PortStructure_t *	DPRX_URxNode_ps;
PortStructure_t *	DPTX_UTxNode_ps;
PortStructure_t *	DPTX_UTXNodeOld_ps;
PortStructure_t *	DPRX_URXNodeOld_ps;

DpTxFeSstMstCommonData_t *TxMstSstFeCommonData_ps;


DpTxSstFeInstanceData_t *TxSstFeData_ps;
DpTxMstFeInstanceData_t *TxMstFeData_ps;
DpTxSstBeInstanceData_t *TxSstBeData_ps;
DpTxMstBeInstanceData_t *TxMstBeData_ps;
DpTxMstBePortSpecificInstanceData_t	*TxMstBePortSpecificData_ps;

DpRxSstFeInstanceData_t *RxSstFeData_ps;
DpRxMstFeInstanceData_t *RxMstFeData_ps;
DpRxSstBeInstanceData_t *RxSstBeData_ps;
DpRxMstBeInstanceData_t *RxMstBeData_ps;
DpRxFeSstMstCommonData_t *RxMstSstFeCommonData_ps;
//******************************************************************************
//	G L O B A L   M A C R O   D E F I N E S
//******************************************************************************
#define gm_SetRegBitsWord(W_RegAddr, W_Set) (*(volatile WORD *)(W_RegAddr) |= (WORD)(W_Set))
#define gm_WriteRegWord(W_RegAddr, W_RegValue) (*(volatile WORD *)(W_RegAddr) = (WORD)(W_RegValue))
#define gm_ReadRegWord(W_RegAddr) (*(volatile WORD *)(W_RegAddr))
#define gm_ReadRegDWord_LE(W_RegAddr) (((DWORD)(gm_ReadRegWord(W_RegAddr+2)) << 16) | gm_ReadRegWord(W_RegAddr))

//******************************************************************************
//	G L O B A L   F U N C T I O N S
//******************************************************************************
void DP_EXT_FUNC DP_Init(DP_PortInitParams_t far* DP_PortInitParams_ps)
{
   U8 i;
   U8 j;
   U8 k;
#if DP_NUM_SST_ONLY_RX != 0
   U8 SstRxPortIndex		= 0;
#endif

#if DP_NUM_SST_ONLY_TX != 0
   U8 SstTxPortIndex		= 0;
#endif

#if DP_RL_NUM_UPSTRM_PORTS != 0
   U8 MstRxPortIndex		= 0;
   U8 RxPhysicalPort_8    = DP_RL_NUM_UPSTRM_PORTS;
#endif

#if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
   U8 MstTxPortIndex		= 0;
   U8 TxPhysicalPort_8    = DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS+DP_RL_NUM_UPSTRM_PORTS;
#endif

#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
   U8 LogPortIndex		= 0;
   U8 LogicalPort_8		= 8;
#endif

#if NUM_OF_TX_FE != 0
   U8 TxFeCommonInstanceIndex = 0;
   U8 NumOfTxFe_8             = NUM_OF_TX_FE;
#endif

#if NUM_OF_RX_FE != 0
   U8 RxFeCommonInstanceIndex = 0;
#endif

   if (DP_PortInitParams_ps == NULL)
   {
      return;
   }
   
   DP_PortList = (PortStructure_t *)0;
   gm_SetRegBitsWord(DP_TOP_CLK_CONFIG, DP_RL_RX0_EN|DP_RL_TX0_EN|DP_RL_TX1_EN|DP_TCLK_EN); // 0x8608 |= 0x3C00
   // gm_SetRegBitsWord(DP_MIXER_IRQ_SELECT, 0xB0); // 0xc848 |= 0xB0
#if DP_RL_NUM_UPSTRM_PORTS != 0
   #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
      {
         DPRL_RlParam_ps = &DPRL_Upkt_s;
         DPRL_RlParam_ps->NumberUpStreamPorts               = DP_RL_NUM_UPSTRM_PORTS;
         DPRL_RlParam_ps->NumberDownStreamPhysicalPorts     = DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS;
         DPRL_RlParam_ps->NumberDownStreamLogicalPorts      = DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS;
         DPRL_RlParam_ps->BridgeTopRegMemOffset_p           = (DP_BaseAddr_t) DP_TOP_CONTROL;
         DPRL_RlParam_ps->RoutingLogicTopRegMemOffset_p     = (DP_BaseAddr_t) DP12_RL_TOP_CTRL;
         #if defined(ST_4K2K_93xx_BOARD) && defined(DP_HUB_2_LOG_PORTS)
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
            DPRL_RlParam_ps->LeftAndRightPanel                 = DP_TRUE;
         else
            DPRL_RlParam_ps->LeftAndRightPanel                 = DP_FALSE;
         #else
         DPRL_RlParam_ps->LeftAndRightPanel                 = DP_FALSE;
         #endif
      }
   //	DP12_InitRoutingLogic();
#endif

#if DP_TOTAL_NUM_PORTS
   {  
      U8 TotNumPorts = DP_TOTAL_NUM_PORTS;
      #ifdef ST_4K2K_93xx_BOARD
      if(GetChipID_4K2K()==CHIPID_4K2K_FE)
      {
         TotNumPorts = 1; //DP_PortType_SstOnlyTx
      }
      else
      {
         TotNumPorts --; //DP_PortType_SstOnlyTx
      }
      
      #endif
      for (i = 0; i < TotNumPorts; i++)
      {
         switch (DP_PortInitParams_ps[i].PortType_e)
         {
         #if DP_NUM_SST_ONLY_RX != 0
            case  DP_PortType_SstOnlyRx:
               AddPortStructureToPortList(&DP_PortList,(PortStructure_t *)&DPRX_SstOnlyPortInstanceData[SstRxPortIndex]);
   			DPRX_SstOnlyPortInstanceData[SstRxPortIndex].FeData_ps = &DPRX_SstFeData[SstRxPortIndex];
   			DPRX_SstOnlyPortInstanceData[SstRxPortIndex].BeData_ps = &DPRX_SstBeData[SstRxPortIndex];
   			DPRX_SstOnlyPortInstanceData[SstRxPortIndex].FeData_ps->DpRxFeSstMstCommonData_ps = &DpRxFeSstMstCommonData_s[RxFeCommonInstanceIndex++];
               DPRX_URxNode_ps = (PortStructure_t *)&DPRX_SstOnlyPortInstanceData[SstRxPortIndex];
               DPRX_URxNode_ps->PortId           = DP_PortInitParams_ps[i].ApplicationPortId_8;
               DPRX_URxNode_ps->PortNum 	      = SST_ONLY_PORT_NUMBER;
               DPRX_URxNode_ps->PortType         = DP_PortInitParams_ps[i].PortType_e;
               DPRX_URxNode_ps->RoutingLogicPortNum_e  = SST_ONLY_HARDWARE_MAP;
               DPRX_URxNode_ps->PortState_e	  = DP_PortState_Active;            

               DP_SetRxInstanceData();
               DP12_InitRxSstPort(DP_PortInitParams_ps[i].PortInitParams_ps);
               SstRxPortIndex++;
               msg("SST only Rx Init",0);			
            break;
         #endif

         #if DP_RL_NUM_UPSTRM_PORTS != 0
            case DP_PortType_MstCapableRx:
               AddPortStructureToPortList(&DP_PortList,(PortStructure_t *)&DPRX_MstPortInstanceData[MstRxPortIndex]);
   			DPRX_MstPortInstanceData[MstRxPortIndex].FeData_ps = &DPRX_MstFeData[MstRxPortIndex];
   			DPRX_MstPortInstanceData[MstRxPortIndex].BeData_ps = &DPRX_MstBeData[MstRxPortIndex];
   			DPRX_MstPortInstanceData[MstRxPortIndex].FeData_ps->DpRxFeSstMstCommonData_ps = &DpRxFeSstMstCommonData_s[RxFeCommonInstanceIndex++];
               DPRX_URxNode_ps = (PortStructure_t *)&DPRX_MstPortInstanceData[MstRxPortIndex];
               DPRX_URxNode_ps->PortId           = DP_PortInitParams_ps[i].ApplicationPortId_8;
               DPRX_URxNode_ps->PortNum 	      = --RxPhysicalPort_8;
               DPRX_URxNode_ps->PortType         = DP_PortInitParams_ps[i].PortType_e;
               DPRX_URxNode_ps->RoutingLogicPortNum_e 		= DP_PortInitParams_ps[i].RoutingLogicPortNum_e;
               DPRX_URxNode_ps->PortState_e	  = DP_PortState_Active;

               DP_SetRxInstanceData();
               DP12_InitRxMstPort(DP_PortInitParams_ps[i].PortInitParams_ps);
               DP_GtcInit(DP_PortType_MstCapableRx);
               MstRxPortIndex++;
               msg("MST Rx Init",0);
            break;
         #endif

         #if DP_NUM_SST_ONLY_TX != 0
            case DP_PortType_SstOnlyTx:
               AddPortStructureToPortList(&DP_PortList,(PortStructure_t *)&DPTX_SstOnlyPortInstanceData[SstTxPortIndex]);
   			   DPTX_SstOnlyPortInstanceData[SstTxPortIndex].BeData_ps 	= &DPTX_SstBeData[SstTxPortIndex];
   			   DPTX_SstOnlyPortInstanceData[SstTxPortIndex].FeData_ps	= &DPTX_SstFeData[SstTxPortIndex];
   			   DPTX_SstOnlyPortInstanceData[SstTxPortIndex].FeData_ps->DpTxFeSstMstCommonData_ps = &DpTxFeSstMstCommonData_s[TxFeCommonInstanceIndex];
               DPTX_UTxNode_ps = (PortStructure_t *)&DPTX_SstOnlyPortInstanceData[SstTxPortIndex];
               DPTX_UTxNode_ps->PortId 	      = DP_PortInitParams_ps[i].ApplicationPortId_8;
               DPTX_UTxNode_ps->PortNum	      = SST_ONLY_PORT_NUMBER;
               DPTX_UTxNode_ps->PortType	      = DP_PortInitParams_ps[i].PortType_e;
               DPTX_UTxNode_ps->RoutingLogicPortNum_e  = SST_ONLY_HARDWARE_MAP;
               
               #ifdef ST_4K2K_93xx_BOARD
               if(GetChipID_4K2K()==CHIPID_4K2K_FE)
                  DPTX_UTxNode_ps->PortState_e	  = DP_PortState_Active;
               #else
                  #if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
                  DPTX_UTxNode_ps->PortState_e	  = DP_PortState_InActive;
                  #else
                  DPTX_UTxNode_ps->PortState_e	  = DP_PortState_Active;
                  #endif
               #endif

               if(TxFeCommonInstanceIndex < NumOfTxFe_8-1)
                  TxFeCommonInstanceIndex++;
               DP_SetTxInstanceData();
               DP12_InitTxSstPort(DP_PortInitParams_ps[i].PortInitParams_ps);
               //gm_Delay10ms(10);
               SstTxPortIndex++;
               msg("SST only Tx Init",0);			
            break;
         #endif

         #if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
            case DP_PortType_MstCapableTx:
   		 	DPTX_MstPortInstanceData[MstTxPortIndex].BeData_ps= &DPTX_MstBeData[MstTxPortIndex];
   			DPTX_MstPortInstanceData[MstTxPortIndex].FeData_ps= &DPTX_MstFeData[MstTxPortIndex];
   			DPTX_MstPortInstanceData[MstTxPortIndex].FeData_ps->DpTxFeSstMstCommonData_ps = &DpTxFeSstMstCommonData_s[TxFeCommonInstanceIndex];
   		 	DPTX_MstPortInstanceData[MstTxPortIndex].BeData_ps->DPTx_MstBePortSpecific_ps = &DPTX_MstPortSpecificBeData[MstTxPortIndex];
   	        AddPortStructureToPortList(&DP_PortList, (PortStructure_t *)&DPTX_MstPortInstanceData[MstTxPortIndex]);
               DPTX_UTxNode_ps = (PortStructure_t *)&DPTX_MstPortInstanceData[MstTxPortIndex];
               DPTX_UTxNode_ps->PortId 	= DP_PortInitParams_ps[i].ApplicationPortId_8;
            #ifdef DP_HUB_2_LOG_PORTS
               #ifdef ST_4K2K_93xx_BOARD
                  if(GetChipID_4K2K()==CHIPID_4K2K_R)
                     DPTX_UTxNode_ps->PortNum	= 9;
                  else
                     DPTX_UTxNode_ps->PortNum	= --TxPhysicalPort_8;   
               #else
                  DPTX_UTxNode_ps->PortNum	= 9;
               #endif
            #else
               DPTX_UTxNode_ps->PortNum	= --TxPhysicalPort_8;
            #endif
               DPTX_UTxNode_ps->PortType	= DP_PortInitParams_ps[i].PortType_e;
               DPTX_UTxNode_ps->RoutingLogicPortNum_e  = DP_PortInitParams_ps[i].RoutingLogicPortNum_e;
               DPTX_UTxNode_ps->PortState_e	  = DP_PortState_Active;

               if(TxFeCommonInstanceIndex < NumOfTxFe_8-1)
                  TxFeCommonInstanceIndex++;
               DP_SetTxInstanceData();
               DP12_InitTxMstPort(DP_PortInitParams_ps[i].PortInitParams_ps);
               DP_GtcInit(DP_PortType_MstCapableTx);
               //gm_Delay10ms(10);
               MstTxPortIndex++;
               msg("MST Tx Init",0);		
            break;
         #endif

         #if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
            case DP_PortType_DigitalTx:
   		 	DPTX_LogPortInstanceData[LogPortIndex].TxBeData_ps = &DPTX_LogPortBeData[LogPortIndex];
   			DPTX_LogPortInstanceData[LogPortIndex].RxBeData_ps = &DPRX_LogPortSstBeData[LogPortIndex];
   		 	DPTX_LogPortInstanceData[LogPortIndex].TxBeData_ps->DPTx_MstBePortSpecific_ps = &DPTX_LogPortSpecificBeData[LogPortIndex];
               AddPortStructureToPortList(&DP_PortList,(PortStructure_t *)&DPTX_LogPortInstanceData[LogPortIndex]);
               DPTX_UTxNode_ps = (PortStructure_t *)&DPTX_LogPortInstanceData[LogPortIndex];
               DPTX_UTxNode_ps->PortId 	= DP_PortInitParams_ps[i].ApplicationPortId_8;
               DPTX_UTxNode_ps->PortNum	= LogicalPort_8;
               DPTX_UTxNode_ps->PortType	= DP_PortInitParams_ps[i].PortType_e;
               DPTX_UTxNode_ps->RoutingLogicPortNum_e  = DP_PortInitParams_ps[i].RoutingLogicPortNum_e;
               DPTX_UTxNode_ps->PortState_e	  = DP_PortState_Active;
               DP_SetTxInstanceData();
               DP12_InitDigitalTransmitter(DP_PortInitParams_ps[i].PortInitParams_ps);
               LogicalPort_8++;
            #if 1   //CDDS2 config
               gm_WriteRegWord(CDDS2_FREQ,0xb6db);
               msg("cdds2=%x",gm_ReadRegWord(CDDS2_FREQ));
               gm_SetRegBitsWord(CLOCK_CONFIG4, DP_BIT15);
               gm_SetRegBitsWord(DP_TOP_CLK_CONFIG, BIT14);
            #endif         
               msg("Log Tx Init",0);
            break;
         #endif
         }
      }
   }
#endif //DP_TOTAL_NUM_PORTS

#if DP_RL_NUM_UPSTRM_PORTS != 0 
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
   {
      DP12_InitRoutingLogic();
      DPBU_GtcInit();      
   }
#endif

   DP_PrintBUVersion();
}

extern void DP12_CrcTestHandler (void);
U8 SendAuxTrans = 0;
extern BOOL DP_EXT_FUNC Tx_Fe_AuxDpcdRead(U32 Addr_32, U8 DP_RAM * InOutBuff_p8, U16 InLen_16);
void DP_EXT_FUNC DP_Handler(void)
{
   U8 i;
   PortStructure_t *	PortList = DP_PortList;

   while (PortList != (PortStructure_t *)0)
   {
   		if(PortList->PortState_e == DP_PortState_Active 
		      || PortList->PortState_e == DP_PortState_TransitionToInActive)
         {

            switch (PortList->PortType)
            {
               #if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
               case DP_PortType_DigitalTx:
                  DPTX_UTxNode_ps = PortList;
                  DP_SetTxInstanceData();
                  DPTX_DigitalTxHandler();
               break;
               #endif

               #if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
               case DP_PortType_MstCapableTx:
                  DPTX_UTxNode_ps = PortList;
                  DP_SetTxInstanceData();
                  DPTX_MstCapablePortHandler();

            	if(SendAuxTrans)
            	{
            		U8 temp[2]={0,0};
            		Tx_Fe_AuxDpcdRead(0x01,temp,2);
            	}
               break;
               #endif

               #if DP_NUM_SST_ONLY_TX != 0
               case DP_PortType_SstOnlyTx:
                  DPTX_UTxNode_ps = PortList;
                  DP_SetTxInstanceData();
                  DPTX_SstOnlyPortHandler();
               break;
               #endif

               #if DP_RL_NUM_UPSTRM_PORTS != 0
               case DP_PortType_MstCapableRx:
                  DPRX_URxNode_ps = PortList;
                  DP_SetRxInstanceData();
                  DPRX_MstCapablePortHandler();
               break;
               #endif

               #if DP_NUM_SST_ONLY_RX != 0
               case DP_PortType_SstOnlyRx:
                  DPRX_URxNode_ps = PortList;
                  DP_SetRxInstanceData();
                  DPRX_SstOnlyPortHandler();
               break;
               #endif
            }
         }
      PortList = (PortStructure_t *)(PortList->NextListItem);
   }
#if DP_RL_NUM_UPSTRM_PORTS!= 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
      BU_Sys_Handler();
#endif

#if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS != 0
   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()!=CHIPID_4K2K_FE)
   #endif
      DP12_TxPortSwitchHandler();
#endif

 //  DP12_CrcTestHandler();	
}

void DP_EXT_FUNC DP_SetRxPower(U8 RxPortId_8, DP_PowerState_t NewPowerState_e)
{
   PortStructure_t *		RxInstance_p		= DP_GetRxInstanceByPortId(RxPortId_8);		// get instance
   PortStructure_t *		RxInstanceOld_p;

   if (RxInstance_p != (PortStructure_t *)NULL)
   {
      RxInstanceOld_p = DPRX_URxNode_ps; // backup instance
      DPRX_URxNode_ps = RxInstance_p; // switch RX pointer
      DP_SetRxInstanceData();

      PM_SetPower(NewPowerState_e);

      DPRX_URxNode_ps = RxInstanceOld_p; //restore
      DP_SetRxInstanceData();
   }
}

void DP_EXT_FUNC DP_SetTxPower (U8 PortId_8, DP_PowerState_t  NewPowerState_e)
{
   DPTX_UTXNodeOld_ps = DPTX_UTxNode_ps ;
   DP12_SetCurrentTxByPortId(PortId_8);
   TX_SetPower(NewPowerState_e);
   DPTX_UTxNode_ps = DPTX_UTXNodeOld_ps;
   DP_SetTxInstanceData();
}

#if DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS !=0 || DP_NUM_SST_ONLY_TX !=0
BOOL DP_EXT_FUNC DP_IsTxActive(U8 PortId_8)
{
   PortStructure_t *		PortStructureData_ps		= DP12_GetPortStructureByPortId(PortId_8);

   //if (PortStructureData_ps->PortState_e <= DP_PortState_TransitionToActive)
   if (PortStructureData_ps->PortState_e == DP_PortState_Active)  //Changed for PATCH_DPTX_AUD_EN_WAIT_FOR_PORT_ACTIVE==1
      return TRUE;
   else
      return FALSE;
}


BOOL DP_EXT_FUNC DP_IsTxHpdHigh(void)
{
   return (gm_ReadRegWord(DP12TX_HPD_IRQ_STATUS) & DPTX_HPD_RAW_STATUS) ? TRUE:FALSE;
}
#endif

#if DP_NUM_SST_ONLY_RX != 0 || DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
void DP_EXT_FUNC DP_GetRxVideoParameters(U8 PortId, DP_MsaParameters_t DP_RAM * MsaParam_ps)
{
   PortStructure_t *		PortStructureData_ps		= DP12_GetPortStructureByPortId(PortId);
	U32 	MS_Nvalue_32;
#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
   if (PortStructureData_ps->PortType == DP_PortType_DigitalTx)
   {
		DP12_SetCurrentTxByPortId(PortId);

		DPRX_GetVideoParameters(MsaParam_ps);
		// TODO: Function should return indication of success or failure -- return from here would be success
#if (PCLK_VALUE_FROM_AVDDS!=0)
      MsaParam_ps->PixelClockHz_32=DpRx_UpdatePixelClkExt(PortId);
#else
		MS_Nvalue_32			= DPRX_GetMvidN();
		if(MS_Nvalue_32 != 0)
		{
		
      #if 1
         U32 LsClk_32;
         if (DPRX_GetMvidM() < 0x100000)
            LsClk_32 = (DPRX_GetMvidM() * 2000) / (MS_Nvalue_32);
         else if (DPRX_GetMvidM() < 0x700000)
            LsClk_32 = ((DPRX_GetMvidM() >> 2) * 2000) / (MS_Nvalue_32 >> 2);
         else
            LsClk_32 = ((DPRX_GetMvidM() >> 3) * 2000) / (MS_Nvalue_32 >> 3);
         LsClk_32 = ((U32)(READ_REG0_U16(CDDS2_FREQ)) * 21 * LsClk_32 / 91) * 25;
         MsaParam_ps->PixelClockHz_32 = LsClk_32;
      #else
         U32 LsClk_32 = ((READ_REG0_U16(CDDS2_FREQ)) * 378UL *2UL)/65536UL;

         MsaParam_ps->PixelClockHz_32  = (LsClk_32 * 1000000UL) / MS_Nvalue_32;
         MsaParam_ps->PixelClockHz_32 *= DPRX_GetMvidM();
		#endif
		
		}
		else
		{
			MsaParam_ps->PixelClockHz_32	= 0;
		}
#endif		
   }
#endif

#if DP_NUM_SST_ONLY_RX != 0
	if (PortStructureData_ps->PortType == DP_PortType_SstOnlyRx)
	{
		DP12_SetCurrentRxByPortId(PortId);

		DPRX_GetVideoParameters(MsaParam_ps);
		// TODO: Function should return indication of success or failure -- return from here would be success

		MS_Nvalue_32			= DPRX_GetMvidN();
		if(MS_Nvalue_32 != 0)
		{
			MsaParam_ps->PixelClockHz_32  = (U32)DPRX_GetLinkRate() * 27000000UL / MS_Nvalue_32;
			MsaParam_ps->PixelClockHz_32 *= DPRX_GetMvidM();
		}
		else
		{
			MsaParam_ps->PixelClockHz_32	= 0;
		}
	}
#endif

   // TODO: Function should return indication of success or failure -- return from here would be failure
}
#else
void DP_EXT_FUNC DP_GetRxVideoParameters(U8 PortId, DP_MsaParameters_t DP_RAM * MsaParam_ps)
{
   //Avoid Compiler Warning
   PortId=PortId;
   MsaParam_ps=MsaParam_ps;
}
#endif

#if DP_NUM_SST_ONLY_RX != 0 || DP_RL_NUM_UPSTRM_PORTS != 0
void DP_EXT_FUNC DP_SetHpdHigh(U8 PortId_8)
{
   PortStructure_t *		PortStructureData_ps		= DP12_GetPortStructureByPortId(PortId_8);
   DP12_SetCurrentRxByPortId(PortId_8);
   if (PortStructureData_ps->PortType == DP_PortType_SstOnlyRx)
   {
      HPD_AppHpdSetHigh();
      HPD_HpdSetHigh();
   }
   else
   {
      HPD_AppHpdSetHigh();
   }
}
void DP_EXT_FUNC DP_SetHpdLow(U8 PortId_8)
{
   PortStructure_t *		PortStructureData_ps		= DP12_GetPortStructureByPortId(PortId_8);
   DP12_SetCurrentRxByPortId(PortId_8);
   if (PortStructureData_ps->PortType == DP_PortType_SstOnlyRx)
   {
      HPD_AppHpdSetLow();
      HPD_HpdSetLow();
   }
   else
   {
      HPD_AppHpdSetLow();
   }
}
#endif

#if DP_NUM_SST_ONLY_RX != 0 || DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
DPRXLSMStatus_t DP_EXT_FUNC DP_GetLinkStatus(U8 PortId_8)
{
   DP12_SetCurrentRxByPortId(PortId_8);
   return LSM_LsmGetStatus();
}



BOOL DP_EXT_FUNC DP_IsRxVideoPresent(U8 PortId_8)
{
   PortStructure_t *		PortStructureData_ps		= DP12_GetPortStructureByPortId(PortId_8);
   DPTX_RxInSel_t	DPTX_RxInSel_e;
   U8 VideoEnabled = DP_FALSE;

#if DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS != 0
   if (PortStructureData_ps->PortType == DP_PortType_DigitalTx)
   {
      DP12_SetCurrentTxByPortId(PortId_8);

      DPTX_RxInSel_e = Tx_Be_GetRxIn();

      if (DPTX_RxInSel_e < DPTX_RxInSel_DpVersion12FromRx0)
      {
         DP12_SetCurrentRxByRoutingLogicPortNum(DPTX_RxInSel_e);
      }
      else
      {
         DP12_SetCurrentRxByRoutingLogicPortNum(DPTX_RxInSel_e-DPTX_RxInSel_DpVersion12FromRx0);
      }

      if ((RxMstSstFeCommonData_ps->DpRxExtFuncGetCableStatus_pf(DPRX_URxNode_ps->PortId) != 0)
            && (LSM_LsmGetStatus()== DPRXLSMStatus_SL_DONE))
      {
         if (DPTX_RxInSel_e < DPTX_RxInSel_DpVersion12FromRx0)
         {
            VideoEnabled = TxMstBePortSpecificData_ps->DTG_Stable;
         }
         else
         {
            VideoEnabled = TxMstBePortSpecificData_ps->VideoEnabled && TxMstBePortSpecificData_ps->DTG_Stable;
         }
      }
      return DPRX_IsVideoPresent(DP_TRUE, VideoEnabled);
   }
#endif

#if DP_NUM_SST_ONLY_RX != 0
   if (PortStructureData_ps->PortType == DP_PortType_SstOnlyRx)
   {
      DP12_SetCurrentRxByPortId(PortId_8);

      return DPRX_IsVideoPresent(DP_FALSE, DP_FALSE);
   }
#endif

   return DP_FALSE;
}

#else
DPRXLSMStatus_t DP_EXT_FUNC DP_GetLinkStatus(U8 PortId_8)
{
   PortId_8=PortId_8;
   return DPRXLSMStatus_NONE;
}


BOOL DP_EXT_FUNC DP_IsRxVideoPresent(U8 PortId_8)
{
   //Avoid Compiler Warning
   PortId_8=PortId_8;
   return FALSE;
}
#endif
void DP_EXT_FUNC DP_ResetRxLink(U8 PortId_8)
{
   DP12_SetCurrentRxByPortId(PortId_8);

   DPRX_LinkReset();
}

/**************************************************************************
FUNCTION: DP_WriteGUID

DESCRIPTION:
Function helps the application set a value for the GUID. Function DP_RX_Fe_WriteGUID does 
the following: 
Since The GUID is one per physical device, 
the GUID is written to all the DPCD GUID location (0x00030-0x0003F) of all RX ports.
A 1 is written to the AUX_GUID_CTRL registers so that further writes to the GUID DPCD 
locations will be prevented.

Notes: On Athena the AUX_GUID_CTRL register for the MST RX port does not work
as designed. So writing 1 to this register has no effect.

INPUT PARAMETERS:
U8 *	: The GUID value to set in the DPCD
U8		: The size of the buffer containing the GUID value

RETURN TYPE:
Boolean
DP_TRUE		: If GUID was written to DPCD location successfully
DP_FALSE	: In case of any errors

****************************************************************************/
BOOL DP_EXT_FUNC DP_WriteGUID(U8 FAR* Guid_Val_p,U8 Num_bytes_8)
{
	
	U8 					i;
	U8					Guid_non_zero_8;
	PortStructure_t* 	PortList;
	DP_PortType_t 		Port_Type_e;

	/* Check if GUID value size is != 16  */
	if ( Num_bytes_8 != 16 )
	{
		/* Nothing to do return */
		return DP_FALSE;
	}

	/* Ensure the GUID value passed to this function is a non-zero value */
	Guid_non_zero_8 = DP_FALSE;
	for(i = 0; i < GUID_SIZE_BYTES; i++)
	{
		if ( Guid_Val_p[i] != 0 )
		{
			Guid_non_zero_8 = DP_TRUE;
			break;
		}
	}
	if (Guid_non_zero_8 == 0)
	{
		return DP_FALSE;
	}

	/* Cycle through the list of ports, if there is an RxPort then
	* write the GUID and lock further writing of the GUID by writing 
	* 1 to the AUX_GUID_CTRL register */
	PortList = DP_GetPortList();
	while (PortList != (PortStructure_t *)0)
	{
		Port_Type_e = GetPortType(&PortList);
		if ( (Port_Type_e == DP_PortType_MstCapableRx) || (Port_Type_e == DP_PortType_SstOnlyRx) )
		{
			DP_RX_FE_WriteGUID(Guid_Val_p);
		}
	}
	
	return DP_TRUE;
}


/**************************************************************************
FUNCTION: DP_GetRx3DFormat

DESCRIPTION:
Function helps the application get the 3D Configuration

INPUT PARAMETERS:
U8					: 	Port number

RETURN TYPE:
STDP_3DFormat_t	:	3DFormat value

****************************************************************************/
STDP_3DFormat_t DP_EXT_FUNC DP_GetRx3DFormat(U8 Port_ID)
{
	DP12_SetCurrentTxByPortId(Port_ID);
	return SDP_Get3DConfig();
}

/**************************************************************************
FUNCTION: DP_SendVSCPacket

DESCRIPTION:
Function helps the application send 3D Information as a VSC packet

INPUT PARAMETERS:
U8					: 	Port number
STDP_3DFormat_t		:	3DFormat value

RETURN TYPE:
None

****************************************************************************/
void DP_EXT_FUNC DP_SendVSCPacket(U8 Port_ID, STDP_3DFormat_t ThreeDFormat)
{
	DP12_SetCurrentTxByPortId(Port_ID);
	SDP_SendVSCPacket(ThreeDFormat);
}

/**************************************************************************
FUNCTION: DP_SetLeftRight

DESCRIPTION:
Function helps the application set left or right frame in the MISC bits.
This is required when the Stereo Interface Method is Frame Sequential & the Interface method
= 0 (means Left and Right View are indicated by MISC1 bits 2:1)

Note: This function can be called only for the SST port because the Tx Registers to
set the View are available on SST port only.

INPUT PARAMETERS:
U8					: 	Port number
STDP_3DView_t		:	3DView value (Left/Right)

RETURN TYPE:
None

****************************************************************************/
void DP_EXT_FUNC DP_SetLeftRight(U8 Port_ID, STDP_3DView_t ThreeDView)
{
	DP12_SetCurrentTxByPortId(Port_ID);
	SDP_SetLeftRight(ThreeDView);
}

//******************************************************************************
//	L O C A L   F U N C T I O N S
//******************************************************************************

void DP_EXT_FUNC DP_Interrupt(void)
{
   U8						TopIrqMask_8			= DPRX_FE0_IRQ_STS;
   U32						DpTopStatus				= READ_REG_U32(DP_TOP_IRQ_STATUS);
   PortStructure_t *		SavedRxNode_ps			= DPRX_URxNode_ps;
   PortStructure_t *		SavedTxNode_ps			= DPTX_UTxNode_ps;

   while (TopIrqMask_8 != 0)
   {
      switch (DpTopStatus & TopIrqMask_8)
      {
         case DPRX_FE0_IRQ_STS:
            DPRX_URxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DP12RX_RL_CONFIG);

            if (DPRX_URxNode_ps!=NULL)
            {
			   DpMstRxPowerManagementBeforeLT();
               DP_SetRxInstanceData();
               DP12RX_Isr();
               SetDp1AuxTrigger();
            }
            break;

         case DPRX_BE0_IRQ_STS:
			DPTX_UTxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DP12TX1_RL_CONFIG);
			if (DPTX_UTxNode_ps !=NULL)
			{
				DP_SetTxInstanceData();
				DPRX_BE_Isr();
				SetDp1AuxTrigger();
			}
            break;

         case DPRX1_FE_IRQ_STS:
            DPRX_URxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DP12RX1_BE_CONTROL);
            if (DPRX_URxNode_ps !=NULL)
            {
               DP_SetRxInstanceData();
               DPRX_Isr();
               SetDp2AuxTrigger();
            }
            break;

         case DPRX_BE1_IRQ_STS:
			DPRX_URxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DP12RX1_BE_CONTROL);
			if (DPRX_URxNode_ps !=NULL)
			{
				DP_SetRxInstanceData();
				DPRX_BE_Isr();
				SetDp2AuxTrigger();
			}
            break;

         case DP_RL_IRQ_STS:
            if (DPRL_RlParam_ps!= NULL)
            {
               DP12RL_Isr();
               SetDp1AuxTrigger();
            }
            break;

         case DPTX_BE_IRQ:
         case DPTX_PHYA_IRQ:
            //case DPTX1_FE_IRQ:
            break;

         case DPTX_FE_IRQ:
            DPTX_UTxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DP12TX2_RL_CONFIG);

            if (DPTX_UTxNode_ps== NULL)
            {
               DPTX_UTxNode_ps = FindPortWithBeBaseAddress((DP_BaseAddr_t)DPTX_BE_CONTROL);
            }
            if (DPTX_UTxNode_ps !=NULL)
            {
               DP_SetTxInstanceData();

               if (DPTX_UTxNode_ps->PortType == DP_PortType_MstCapableTx)
               {
                  DP12TX_Isr();
               }
               else
               {
                  DPTX_Isr();
               }
            }
            break;
      }

      TopIrqMask_8 <<= 1;
   }
   DPRX_URxNode_ps = SavedRxNode_ps;
   DPTX_UTxNode_ps = SavedTxNode_ps;
   DP_SetTxInstanceData();
   DP_SetRxInstanceData();
}

DP_BaseAddr_t DP_EXT_FUNC DP_GetBeBaseAddress (DP_RLBaseAddress_t BeAddress_e)
{
   switch (BeAddress_e)
   {
      case DP12RX0_RL_CONFIG_ADDRESS:
         return (DP_BaseAddr_t)DP12RX_RL_CONFIG;

      case DP12TX0_RL_CONFIG_ADDRESS:
         return (DP_BaseAddr_t)DP12TX1_RL_CONFIG;

      case DP12TX1_RL_CONFIG_ADDRESS:
         return (DP_BaseAddr_t)DP12TX2_RL_CONFIG;

      default:
         return NULL;
   }
}


#endif


