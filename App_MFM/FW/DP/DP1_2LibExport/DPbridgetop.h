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

#ifndef _DPBRIDGETOP_H_
#define _DPBRIDGETOP_H_
//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif

/****************************************************************************/
/*  R E G I S T E R S   D E F I N I T I O N S                               */
/****************************************************************************/

#define DP12_BRIDGE_CTRL                      (Dp12BridgeTopOffset + 0x0)	//0 (DP12_BRG_TOP)
#define DP12_BRIDGE_IRQ_EN                    (Dp12BridgeTopOffset + 0x4)	//4 (DP12_BRG_TOP)
#define DP12_BRIDGE_IRQ_STS                   (Dp12BridgeTopOffset + 0x8)	//8 (DP12_BRG_TOP)
#define DP12_BRIDGE_TX_UART_MUX               (Dp12BridgeTopOffset + 0xC)	//12 (DP12_BRG_TOP)


/****************************************************************************/
/*  B I T S  D E F I N I T I O N S                                          */
/****************************************************************************/


// DP12_BRIDGE_CTRL                           (0x0)
#define DP12_BRIDGE_CTRL_RSVD                 0XFFFF

// DP12_BRIDGE_IRQ_EN                         (0x4)
#define DP12_DPRX0_IRQ_EN                     DP_BIT0
#define DP12_DPRX1_IRQ_EN                     DP_BIT1
#define DP12_DPRX2_IRQ_EN                     DP_BIT2
#define DP12_DPRX3_IRQ_EN                     DP_BIT3
#define DP12_DPTX0_IRQ_EN                     DP_BIT4
#define DP12_DPTX1_IRQ_EN                     DP_BIT5
#define DP12_DPTX2_IRQ_EN                     DP_BIT6
#define DP12_DPTX3_IRQ_EN                     DP_BIT7
#define DP12_RL_IRQ_EN                        DP_BIT8
#define DP12_GTC_IRQ_EN                       DP_BIT9
#define DP12_BRIDGE_IRQ_RSVD                  0XFC00

// DP12_BRIDGE_IRQ_STS                        (0x8)
#define DP12_DPRX0_IRQ_STS                    DP_BIT0
#define DP12_DPRX1_IRQ_STS                    DP_BIT1
#define DP12_DPRX2_IRQ_STS                    DP_BIT2
#define DP12_DPRX3_IRQ_STS                    DP_BIT3
#define DP12_DPTX0_IRQ_STS                    DP_BIT4
#define DP12_DPTX1_IRQ_STS                    DP_BIT5
#define DP12_DPTX2_IRQ_STS                    DP_BIT6
#define DP12_DPTX3_IRQ_STS                    DP_BIT7
#define DP12_RL_IRQ_STS                       DP_BIT8
#define DP12_GTC_IRQ_STS                      DP_BIT9

// DP12_BRIDGE_TX_UART_MUX                    (0xC)
#define DP12_TX0_UART_MUX                     0X0F
#define DP12_TX1_UART_MUX                     0X0F00
#define DP12_TX2_UART_MUX                     0XF0000
#define DP12_TX3_UART_MUX                     0XF000000

#ifdef __PARADIGM__
#pragma pack(pop)
#endif

#endif
