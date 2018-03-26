/*
$Workfile:   DisplayPortConfig.h  $
$Revision: 1.6 $
$Date: 2013/01/08 04:07:21 $
*/
//*****************************************************************************************
//*** COPYRIGHT (C) STMicroelectronics 2010 All rights reserved.                        ***
//*** This document contains proprietary and confidential information of the            ***
//*** STMicroelectronics Group. This document is not to be copied in whole or part.     ***
//*** STMicroelectronics assumes no responsibility for the consequences of use of       ***
//*** such information nor for any infringement of patents or other rights of third     ***
//*** parties which may result from its use. No license is granted by implication       ***
//*** or otherwise under any patent or patent rights of STMicroelectronics.             ***
//*** STMicroelectronics products are not authorized for use as critical components     ***
//*** in life support devices or systems without express written approval of            ***
//*** STMicroelectronics.                                                               ***
//*****************************************************************************************
//***                                                                                   ***
//*** MODULE:DisplayPortConfig.h                                                        ***
//***                                                                                   ***
//*** USAGE:The module contains Definitions used for DisplayPort Configuration          ***
//***                                                                                   ***
//*****************************************************************************************


//*****************************************************************************************
//  G L O B A L    D E F I N I T I O N S
//*****************************************************************************************

#ifndef __DISPLAYPORTCONFIG_MAIN_H__
#define __DISPLAYPORTCONFIG_MAIN_H__
#ifndef __SYSTEM_H__
#include "System\System.h"      // System specific - top of system files
#endif

#define NO_DPTX_OUTPUT              0              // DPTX OUTPUT is disable
#define USE_MST_DPTX_OUTPUT         1              // DPTX is fixed in the path of daisy-chain from Routing Logic 
#define USE_IDP_DPTX_OUTPUT         2              // DPTX is fixed in the path of ODP 
#define USE_DPTX_DYNAMIC_SWITCH     3              // DPTX will be switched to the path of daisy-chain from RL when input port is selected to DpRx0,
                                                   // And, DPTX will be switched to ODP path when the other input ports are selected.
#define USE_DPTX_MANUAL_SWITCH      4              // DPTX is manual switched by application such as OSD control

#if   (defined EV1_93xx_BOARD)
#include    	"Board\EV1-93xx\DisplayPortConfig.h"
#elif (defined EV2_93xx_BOARD)
#include    	"Board\EV2-93xx\DisplayPortConfig.h"
#elif ((defined RD1_93xx_BOARD)||(defined RD3_93xx_BOARD))
#include    	"Board\RD1-93xx\DisplayPortConfig.h"
#elif (defined ST_4K2K_93xx_BOARD)
#include    	"Board\ST4K2K\DisplayPortConfig.h"
#else
#include    	"Board\RD1-93xx\DisplayPortConfig.h"
#endif

//*****************************************************************************************
//  SUB DISPLAY PORT DEFINITIONS ---> Do not modify if no special usage
//*****************************************************************************************
#if NUM_OF_CNTR_ON_CPHY0
#define DP_RL_NUM_UPSTRM_PORTS                     1
#define DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS         1
#else
#define DP_RL_NUM_UPSTRM_PORTS                     0
#define DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS         0
#endif

#if NUM_OF_CNTR_ON_CPHY2
#define DP_NUM_SST_ONLY_RX                         1
#else
#define DP_NUM_SST_ONLY_RX                         0
#endif

#if (SELECT_DPTX_TYPE==USE_MST_DPTX_OUTPUT)
#define DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS        1
#define DP_NUM_SST_ONLY_TX                         0
#elif (SELECT_DPTX_TYPE==USE_IDP_DPTX_OUTPUT)
#define DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS        0
#define DP_NUM_SST_ONLY_TX                         1
#elif ((SELECT_DPTX_TYPE==USE_DPTX_DYNAMIC_SWITCH) || (SELECT_DPTX_TYPE==USE_DPTX_MANUAL_SWITCH))
#define DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS        1
#define DP_NUM_SST_ONLY_TX                         1
#else // (SELECT_DPTX_TYPE==NO_DPTX_OUTPUT)
#define DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS        0
#define DP_NUM_SST_ONLY_TX                         0
#endif

#define DP_RL_NUM_DWNSTRM_PORTS                    ((DP_RL_NUM_DOWNSTREAM_PHYSICAL_PORTS) + (DP_RL_NUM_DOWNSTREAM_LOGICAL_PORTS))
#define DP_TOTAL_NUM_DWNSTRM_PORTS                 ((DP_RL_NUM_DWNSTRM_PORTS) + (DP_NUM_SST_ONLY_TX))
#define DP_TOTAL_NUM_UPSTRM_PORTS                  ((DP_RL_NUM_UPSTRM_PORTS) + (DP_NUM_SST_ONLY_RX))
#define DP_TOTAL_NUM_PORTS                         ((DP_TOTAL_NUM_DWNSTRM_PORTS) + (DP_TOTAL_NUM_UPSTRM_PORTS))

#define NUM_OF_DP_INP                              (NUM_OF_CNTR_ON_CPHY0 +	NUM_OF_CNTR_ON_CPHY2)

#if (SELECT_DPTX_TYPE==NO_DPTX_OUTPUT)
#define NUM_OF_TX_FE                               0
#else
#define NUM_OF_TX_FE                               1 //Shared between SST Only Tx and MST Cap Tx
#endif
#define NUM_OF_RX_FE                               DP_TOTAL_NUM_UPSTRM_PORTS

//*****************************************************************************************
//  SUB DISPLAY PORT DEFINITIONS END
//*****************************************************************************************


#endif // __DISPLAYPORTCONFIG_MAIN_H__
//*****************************************  END  *****************************************

