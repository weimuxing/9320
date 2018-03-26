/*
	$Workfile:   gm_Mcu186.h  $
	$Revision: 1.3 $
	$Date: 2011/12/14 03:44:47 $
*/
#ifndef __GM_MCU186_H__
#define __GM_MCU186_H__
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
// MODULE:      gm_mcu186.h
//
// USAGE:       This module is for MCU Turbo-186 registers definition.
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//
//	PCB (Periferal Control Block) Base Address
//
#define PCB_BASE	0xFA00					// PCB base address	

//
//	PCB (Periferal Control Block) Register Map
//
//
// 	Processor Control Registers
//
#define RELREG				(PCB_BASE + 0xFE) 	// PCB Relocation              	
#define RESCON				(PCB_BASE + 0xF6) 	// Reset Configuration         	
#define PRL					(PCB_BASE + 0xF4) 	// Processor Release Level     	
#define AUXCON				(PCB_BASE + 0xF2) 	// Auxiliary Configuration
#define SYSCON				(PCB_BASE + 0xF0) 	// System Configuration        	
#define WDTCON				(PCB_BASE + 0xE6) 	// Watchdog timer control      	
#define EDRAM     			(PCB_BASE + 0xE4) 	// Enable RCU
#define CDRAM     			(PCB_BASE + 0xE2) 	// Clock Prescaler             	
#define MDRAM     			(PCB_BASE + 0xE0) 	// Memory Partition            	
//
// DMA Registers
//
#define D1CON				(PCB_BASE + 0xDA) 	// DMA1 Control                	
#define D1TC				(PCB_BASE + 0xD8) 	// DMA1 Transfer Count         	
#define D1DSTH				(PCB_BASE + 0xD6) 	// DMA1 Destination Addr. High 	
#define D1DSTL				(PCB_BASE + 0xD4) 	// DMA1 Destination Addr. Low  	
#define D1SRCH				(PCB_BASE + 0xD2) 	// DMA1 Source Addr. High      	
#define D1SRCL				(PCB_BASE + 0xD0) 	// DMA1 Source Addr. Low       	
#define D0CON				(PCB_BASE + 0xCA) 	// DMA0 Control                	
#define D0TC				(PCB_BASE + 0xC8) 	// DMA0 Transfer Count         	
#define D0DSTH				(PCB_BASE + 0xC6) 	// DMA0 Destination Addr. High 	
#define D0DSTL				(PCB_BASE + 0xC4) 	// DMA0 Destination Addr. Low  	
#define D0SRCH				(PCB_BASE + 0xC2) 	// DMA0 Source Addr. High      	
#define D0SRCL				(PCB_BASE + 0xC0) 	// DMA0 Source Addr. Low       	
//
// DMA2 & 3
// Hardware specified, enabled for on Cortez
//
#define D2CON				(PCB_BASE + 0xBA) 	// DMA2 Control                	
#define D2TC				(PCB_BASE + 0xB8) 	// DMA2 Transfer Count         	
#define D2DSTH				(PCB_BASE + 0xB6) 	// DMA2 Destination Addr. High 	
#define D2DSTL				(PCB_BASE + 0xB4) 	// DMA2 Destination Addr. Low  	
#define D2SRCH				(PCB_BASE + 0xB2) 	// DMA2 Source Addr. High      	
#define D2SRCL				(PCB_BASE + 0xB0) 	// DMA2 Source Addr. Low       	
#define D3CON				(PCB_BASE + 0x9A) 	// DMA3 Control                	
#define D3TC				(PCB_BASE + 0x98) 	// DMA3 Transfer Count         	
#define D3DSTH				(PCB_BASE + 0x96) 	// DMA3 Destination Addr. High 	
#define D3DSTL				(PCB_BASE + 0x94) 	// DMA3 Destination Addr. Low  	
#define D3SRCH				(PCB_BASE + 0x92) 	// DMA3 Source Addr. High      	
#define D3SRCL				(PCB_BASE + 0x90) 	// DMA3 Source Addr. Low       	
//
// Chip-Select Registers
//
#define MPCS				(PCB_BASE + 0xA8) 	// PCS and MCS Auxiliary       	
#define MMCS				(PCB_BASE + 0xA6) 	// Midrange Memory Chip Select 	
#define PACS				(PCB_BASE + 0xA4) 	// Peripheral Chip-Select      	
#define LMCS				(PCB_BASE + 0xA2) 	// Low Memory Chip-Select      	
#define UMCS				(PCB_BASE + 0xA0) 	// Upper Memory Chip-Select
//
// Serial Port0 Registers
//
#define SP0BAUD   			(PCB_BASE + 0x88) 	// Serial Port0 Baud Rate Div. 	
#define SP0RD     			(PCB_BASE + 0x86) 	// Serial Port0 Receive        	
#define SP0TD     			(PCB_BASE + 0x84) 	// Serial Port0 Transmit       	
#define SP0STS    			(PCB_BASE + 0x82) 	// Serial Port0 Status         	
#define SP0CT     			(PCB_BASE + 0x80) 	// Serial Port0 Control        	
//
// PIO Registers
//
#define PDATA1				(PCB_BASE + 0x7A) 	// PIO Data 1					
#define PDIR1				(PCB_BASE + 0x78) 	// PIO Direction 1             	
#define PIOMODE1			(PCB_BASE + 0x76) 	// PIO Mode 1                  	
#define PDATA0				(PCB_BASE + 0x74) 	// PIO Data 0                  	
#define PDIR0				(PCB_BASE + 0x72) 	// PIO Direction 0             	
#define PIOMODE0			(PCB_BASE + 0x70) 	// PIO Mode 0                  	
//
// Timer Registers
//
#define T2CON				(PCB_BASE + 0x66) 	// Timer2 Mode/Control         	
#define T2CMPA				(PCB_BASE + 0x62) 	// Timer2 Max Count Compare A  	
#define T2CNT				(PCB_BASE + 0x60) 	// Timer2 Count                	
#define T1CON				(PCB_BASE + 0x5E) 	// Timer1 Mode/Control         	
#define T1CMPB				(PCB_BASE + 0x5C) 	// Timer1 Max Count Compare B
#define T1CMPA				(PCB_BASE + 0x5A) 	// Timer1 Max Count Compare A  	
#define T1CNT				(PCB_BASE + 0x58) 	// Timer1 Count                	
#define T0CON				(PCB_BASE + 0x56) 	// Timer0 Mode/Control         	
#define T0CMPB				(PCB_BASE + 0x54) 	// Timer0 Max Count Compare B  	
#define T0CMPA				(PCB_BASE + 0x52) 	// Timer0 Max Count Compare A  	
#define T0CNT				(PCB_BASE + 0x50) 	// Timer0 Count                	
//
// Interrupt Registers
//
#define DMA3CON				(PCB_BASE + 0x4A) 	// DMA3 	Interrupt Control 	
#define DMA2CON				(PCB_BASE + 0x48) 	// DMA2		Interrupt Control  	
#define I5CON				(PCB_BASE + 0x46) 	// INT5     Interrupt Control  	
#define SP0CON				(PCB_BASE + 0x44) 	// Serial Port0 Interrupt Ctl. 	
#define SP1CON				(PCB_BASE + 0x42) 	// Serial Port1 Interrupt Ctl. 	
#define I4CON				(PCB_BASE + 0x40) 	// INT4     Interrupt Control  	
#define I3CON				(PCB_BASE + 0x3E) 	// INT3     Interrupt Control  	
#define I2CON				(PCB_BASE + 0x3C) 	// INT2     Interrupt Control  	
#define I1CON				(PCB_BASE + 0x3A) 	// INT1     Interrupt Control
#define I0CON				(PCB_BASE + 0x38) 	// INT0     Interrupt Control  	
#define DMA1CON				(PCB_BASE + 0x36) 	// DMA1 	Interrupt Control  	
#define DMA0CON				(PCB_BASE + 0x34) 	// DMA0		Interrupt Control  	
#define TCUCON				(PCB_BASE + 0x32) 	// Timer    Interrupt Control  	
#define INTSTS				(PCB_BASE + 0x30) 	// Interrupt Status            	
#define REQST				(PCB_BASE + 0x2E) 	// Interrupt Request           	
#define INSERV				(PCB_BASE + 0x2C) 	// Interrupt In-Service
#define PRIMSK				(PCB_BASE + 0x2A) 	// Interrupt Priority Mask     	
#define IMASK				(PCB_BASE + 0x28) 	// Interrupt Mask              	
#define POLLST				(PCB_BASE + 0x26) 	// Interrupt Poll Status       	
#define POLL				(PCB_BASE + 0x24) 	// Interrupt Poll              	
#define EOI					(PCB_BASE + 0x22) 	// End-Of-Interrupt				
#define INTVEC				(PCB_BASE + 0x20) 	// Interrupt Vector            	
//
// Serial Port1 Registers (available in ES version - Cortez)
//
#define SP1BAUD   			(PCB_BASE + 0x18) 	// Serial Port1 Baud Rate Div. 	
#define SP1RD     			(PCB_BASE + 0x16) 	// Serial Port1 Receive        	
#define SP1TD     			(PCB_BASE + 0x14) 	// Serial Port1 Transmit       	
#define SP1STS    			(PCB_BASE + 0x12) 	// Serial Port1 Status         	
#define SP1CT     			(PCB_BASE + 0x10) 	// Serial Port1 Control        	

//******************************************************************************
//	PCB (Periferal Control Block) Register Bit Definitions
//******************************************************************************

// SP0CT Serial Port0 Control	(PCB_BASE + 0x80)
// SP1CT Serial Port1 Control 	(PCB_BASE + 0x10)
//DataMode					BIT0				// Bit0 of DataMode
//DataMode					BIT1				// Bit1 of DataMode
//DataMode					BIT2				// Bit2 of DataMode
#define PE  				BIT3				// Parity Enable
#define EVN                	BIT4              	// Even Parity
#define RMODE              	BIT5              	// Receive Mode
#define TMODE             	BIT6              	// Transmit Mode
#define RXIE               	BIT7              	// Receive Data Ready Interrupt Enable
#define TXIE               	BIT8              	// Transmitter  Ready Interrupt Enable
#define FC                 	BIT9              	// Flow Control Enable
#define TB8                	BIT10             	// Transmit Bit 8
#define BRK                	BIT11             	// Send Break
#define RSIE               	BIT12             	// Receive Status Enable
//DMA						BIT13				// DMA Control Bit 0
//DMA                      	BIT14				// DMA Control Bit 1
//DMA						BIT15				// DMA Control Bit 2

// Serial Port Mode settings
#define DATAMODE1			1					// Mode1: 7 or 8 bits; 1 or 0 parity bits; 1 stop bit
#define DATAMODE2			2                 	// Mode2: 9 bits     ; N/A               ; 1 stop bit
#define DATAMODE3			3                 	// Mode3: 8 or 9 bits; 1 or 0 parity bits; 1 stop bit
#define DATAMODE4			4                 	// Mode4: 7 bits     ; N/A               ; 1 stop bit
// DMA operation
#define RxNoDMATxNoDMA		0x0000            	// Receive: NoDMA; Transmit: NoDMA
#define RxDMA0TxDMA1		0x2000            	// Receive: DMA0 ; Transmit: DMA1
#define RxDMA1TxDMA0		0x4000            	// Receive: DMA1 ; Transmit: DMA0
#define RxDMA0TxNoDMA     	0x8000            	// Receive: DMA0 ; Transmit: NoDMA
#define RxDMA1TxNoDMA		0xA000            	// Receive: DMA1 ; Transmit: NoDMA
#define RxNoDMATxDMA0		0xC000            	// Receive: NoDMA; Transmit: DMA0
#define RXNoDMATxDMA1		0xE000            	// Receive: NoDMA; Transmit: DMA1

// SP0STS Serial Port0 Status Register	(PCB_BASE + 0x82)
// SP1STS Serial Port1 Status Register	(PCB_BASE + 0x12)
#define HSO					BIT1              	// Handshake Signal 0
#define TEMT				BIT2              	// Transmitter Empty
#define PER					BIT3              	// Parity Error Detected
#define OER					BIT4              	// Overrun Error Detected
#define FER					BIT5              	// Framing Error Detected
#define THRE				BIT6              	// Transmit Holding Register Empty
#define RDR					BIT7              	// Receive Data Ready
#define RB8					BIT8              	// Received Bit 8
#define BRK0				BIT9              	// Short Break Detected
#define BRK1				BIT10             	// Long  Break Detected
//Reserved					BIT11				//
//Reserved					BIT12				//
//Reserved					BIT13				//
//Reserved					BIT14				//
//Reserved					BIT15				//

// T0CON Timer0 Mode and Control Register	(PCB_BASE + 0x56)
// T1CON Timer1 Mode and Control Register	(PCB_BASE + 0x5E)
#define CONT				BIT0				// Continuous Mode
#define ALT					BIT1				// Alternate Compare
#define EXT					BIT2				// External Clock
#define P					BIT3				// Prescaler Bit
#define RTG					BIT4				// Retrigger Bit
#define MC					BIT5				// Maximum Count Bit
//Reserved					BIT6				// Set to zero
//Reserved					BIT7				// Set to zero
//Reserved					BIT8				// Set to zero
//Reserved					BIT9				// Set to zero
//Reserved					BIT10				// Set to zero
//Reserved					BIT11				// Set to zero
#define RIU					BIT12				// Register in Use Bit
#define INT					BIT13				// Interrupt Bit
#define INH					BIT14				// Inhibit   Bit
#define EN					BIT15				// Enable    Bit

// IMASK Interrupt Mask Register	(PCB_BASE + 0x28)
#define TMR					BIT0				// Timer 0, 1 1nd 2 Interrupt Mask
//Reserved					BIT1				// Set to zero
#define D0					BIT2				// DMA0 Chanel Interrupt Mask
#define D1					BIT3				// DMA1 Chanel Interrupt Mask
#define I0					BIT4				// INT0 Mask
#define I1					BIT5				// INT1 Mask
#define I2					BIT6				// INT2 Mask
#define I3					BIT7				// INT3 Mask
#define I4					BIT8				// INT4 Mask
#define SP1					BIT9				// Serial Port1 Mask
#define SP0					BIT10				// Serial Port0 Mask
#define I5					BIT11				// INT5 Mask
#define D2					BIT12				// DMA0 Chanel Interrupt Mask
#define D3					BIT13				// DMA0 Chanel Interrupt Mask
//Reserved					BIT14				// Set to zero
//Reserved					BIT15				// Set to zero

//******************************************************************************
//					INTERRUPT VECTORS   ADDRESSES
//******************************************************************************

#define DIV_ERR_VECTOR		0x0000				//
#define SINGLE_STEP_VECTOR	0x0004				//
#define NMI_VECTOR			0x0008				//
#define SWBP_VECTOR			0x000C				//
#define INTO_VECTOR			0x0010				//
#define BOUNDS_VECTOR		0x0014				//
#define OPCODE_ERR_VECTOR	0x0018				//
#define ESC_VECTOR			0x001C				//
#define TIMER0_VECTOR		0x0020				//
#define TIMER1_VECTOR		0x0048            	//
#define TIMER2_VECTOR		0x004C            	//
#define DMA0_VECTOR			0x0028            	//
#define DMA1_VECTOR			0x002C            	//
#define DMA2_VECTOR			0x0058            	//
#define DMA3_VECTOR			0x005C            	//
#define INT0_VECTOR			0x0030            	//
#define INT1_VECTOR			0x0034            	//
#define INT2_VECTOR			0x0038            	//
#define INT3_VECTOR			0x003C            	//
#define INT4_VECTOR			0x0040            	//
#define INT5_VECTOR			0x0054            	//
#define UART0_VECTOR		0x0050            	//
#define UART1_VECTOR		0x0044            	//

//******************************************************************************
//					INTERRUPT   VECTOR    TYPES
//******************************************************************************

#define DIV_ERR_VTYPE		0                	//
#define SINGLE_STEP_VTYPE	1					//
#define NMI_VTYPE			2                	//
#define SWBP_VTYPE			3					//
#define INTO_VTYPE			4					//
#define BOUNDS_VTYPE		5					//
#define OPCODE_ERR_VTYPE	6                	//
#define ESC_VTYPE			7					//
#define TIMER0_VTYPE		8                	//
#define TIMER1_VTYPE		18                	//
#define TIMER2_VTYPE		19                	//
#define DMA0_VTYPE			10                	//
#define DMA1_VTYPE			11                	//
#define DMA2_VTYPE			22                	//
#define DMA3_VTYPE			23                	//
#define INT0_VTYPE			12                	//
#define INT1_VTYPE			13                	//
#define INT2_VTYPE			14                	//
#define INT3_VTYPE			15                	//
#define UART1_VTYPE			17                	//
#define INT4_VTYPE			16                	//
#define INT5_VTYPE			21                	//
#define UART0_VTYPE			20                	//

//******************************************************************************
//				DMA DEFINITIONS
//******************************************************************************

enum
{
   DMA_CHANNEL0_NOT_AVAILABLE,
   DMA_CHANNEL0_AVAILABLE,
   DMA_CHANNEL0_AVAILABLE_AFTER_RESET,
   DMA_CHANNEL0_FSB_FIFO_TRANSFER
};

//****EXTERN**** extern BYTE gB_DmaChannel0;

//******************************************************************************
//					DMA CONTROL REGISTER BIT DEFINITIONS
//******************************************************************************

#define DMA_DEST_POINTER_IS_MEMORY	BIT15
#define DMA_DEST_POINTER_IS_IO		0
#define DMA_DEST_POINTER_DEC		BIT14
#define DMA_DEST_POINTER_NOT_DEC	0
#define DMA_DEST_POINTER_INC		BIT13
#define DMA_DEST_POINTER_NOT_INC	0

#define DMA_SRC_POINTER_IS_MEMORY	BIT12
#define DMA_SRC_POINTER_IS_IO		0
#define DMA_SRC_POINTER_DEC			BIT11
#define DMA_SRC_POINTER_NOT_DEC		0
#define DMA_SRC_POINTER_INC			BIT10
#define DMA_SRC_POINTER_NOT_INC		0

#define DMA_TC_TERMINATE			BIT9
#define DMA_TC_NOT_TERMINATE		0

#define DMA_INT_ENABLE_ON_TERM		BIT8
#define DMA_INT_DISABLE_ON_TERM		0

#define DMA_NO_SYNC					0
#define DMA_SRC_SYNC				BIT6
#define DMA_DEST_SYNC				BIT7

#define DMA_HIGH_PRIORITY			BIT5
#define DMA_LOW_PRIORITY			0

#define DMA_ENABLE_TIMER_REQ		BIT4
#define DMA_DISABLE_TIMER_REQ		0

#define DMA_ENABLE_STOP_BIT_UPDATE	BIT2
#define DMA_DISABLE_STOP_BIT_UPDATE	0

#define DMA_START_OPERATIONS		BIT1
#define DMA_STOP_OPERATIONS			0

#define DMA_WORD_TRANSFERS			BIT0
#define DMA_BYTE_TRANSFERS			0

//******************************************************************************
//  M A C R O   D E F I N I T I O N S
//******************************************************************************
#define ENABLE_INTERRUPTS		asm{ sti }
#define DISABLE_INTERRUPTS		asm{ cli }

// Read/Write to specified WORD in given PCB register
#define WRITE_PCB_REG(W_RegAddr, W_RegValue)	\
	((WORD)*((WORD pcbreg* const)(W_RegAddr)) = (WORD)(W_RegValue))

#define READ_PCB_REG(W_RegAddr)					\
	((volatile WORD)*((WORD pcbreg* const)(W_RegAddr)))

// Set/Clear the specified bits in given PCB register
#define SET_PCB_REG_BITS(W_RegAddr, W_RegValue)		\
	((WORD)*((WORD pcbreg* const)(W_RegAddr)) = (volatile WORD)*((WORD pcbreg* const)(W_RegAddr)) | (W_RegValue))

#define CLEAR_PCB_REG_BITS(W_RegAddr, W_RegValue)	\
 	((WORD)*((WORD pcbreg* const)(W_RegAddr)) = (volatile WORD)*((WORD pcbreg* const)(W_RegAddr)) & (~(W_RegValue)))

// Set interrupt vector address
// NOTE:  The high word of the W_CodeAddr is CS, the compiler will copy
//        the CS value to the high word of W_VectAddr.
#define SET_VECTOR(W_VectAddr, W_CodeAddr)	\
	*((WORD*)(W_VectAddr))		= (WORD)(W_CodeAddr); \
	*((WORD*)(W_VectAddr) + 1)	= (WORD) ( ((DWORD)(W_CodeAddr))>>16 )

#endif
//*********************************  END  **************************************
