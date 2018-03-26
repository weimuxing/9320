/*
$Workfile:   DisplayPortConfig.h  $
$Revision: 1.11 $
$Date: 2012/08/22 07:39:39 $
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

#ifndef __DISPLAYPORTCONFIG_H__
#define __DISPLAYPORTCONFIG_H__
//*****************************************************************************************
//  MAJOR DISPLAY PORT DEFINITIONS ---> Need to defined based on board configration by User
//*****************************************************************************************
#ifdef MUX_TEST_BOARD
#define EXTERNAL_MUX0_AVAILABLE        1           //  1: An exteranl mux in front of CPHY0 
#define NUM_OF_CNTR_ON_CPHY0           2           //  Number of physical connectors on CPHY0
                                                   //  If not used, set to 0
#else
#define EXTERNAL_MUX0_AVAILABLE        0           //  1: An exteranl mux in front of CPHY0 
#define NUM_OF_CNTR_ON_CPHY0           1           //  Number of physical connectors on CPHY0
                                                   //  If not used, set to 0
#endif

#define EXTERNAL_MUX2_AVAILABLE        0           //  1: An exteranl mux in front of CPHY2 
#define NUM_OF_CNTR_ON_CPHY2           0           //  Number of physical connectors on CPHY2
                                                   //  If not used, set to 0

#if (FEATURE_DP_HUB == ENABLE)
#define SELECT_DPTX_TYPE      USE_MST_DPTX_OUTPUT
///#elif (defined(RD3_93xx_BOARD) || defined(RD2_93xx_BOARD))
//#define SELECT_DPTX_TYPE      USE_DPTX_DYNAMIC_SWITCH
#else //RD1 
	#if(defined(BOARD_93XX_SUV))		//MNT, SUV, 
		#define SELECT_DPTX_TYPE	  USE_IDP_DPTX_OUTPUT		//For LG 27" eDP panel
		//#define SELECT_DPTX_TYPE	  USE_DPTX_DYNAMIC_SWITCH
	#else
		#define SELECT_DPTX_TYPE	  USE_IDP_DPTX_OUTPUT		//For LG 27" eDP panel
		//#define SELECT_DPTX_TYPE	  USE_DPTX_DYNAMIC_SWITCH//USE_DPTX_DYNAMIC_SWITCH//HYB121210 Org = USE_MST_DPTX_OUTPUT 	 //  Options: NO_DPTX_OUTPUT/ 
	#endif
#endif
                                                       //           USE_MST_DPTX_OUTPUT/
                                                       //           USE_IDP_DPTX_OUTPUT/
                                                       //           USE_DPTX_DYNAMIC_SWITCH 
                                                       //           USE_DPTX_MANUAL_SWITCH
//*****************************************************************************************
//  MAJOR DISPLAY PORT DEFINITIONS END
//*****************************************************************************************
#endif // __DISPLAYPORTCONFIG_H__
//*****************************************  END  *****************************************
