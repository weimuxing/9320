/*
   $Workfile:   gm_DrvLib.h  $
   $Revision: 1.36 $
   $Date: 2013/05/14 06:00:03 $
*/

//*************************************************************************************************
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
// MODULE:      gm_DrvLib.h
//
// USAGE:       That is header file, that is contain declaration from gm_Drv.lib file
//
//*************************************************************************************************

#ifndef __GM_DRVLIB_H__
#define __GM_DRVLIB_H__

#pragma option -b-        //swithes off option "allocate enumerators as integer"

typedef enum
{
   IRQ0 = 0,
   IRQ1 = 1,
   IRQ2 = 2,
   IRQ3 = 3,
   IRQ1_IR_DECORDER  = 4,
   IRQ1_DDC2BI = 5,
   IRQ1_FSB_WR_DONE  = 6,
   IRQ1_SSLAVE       = 8, // TODO: have ?
   IRQ2_RS_READY = 9,
   IRQ4 = 14,
   IRQ5 = 15,
} gmt_IsrName;

//*************************************************************************************************
// Common error number for function return status
//*************************************************************************************************
typedef unsigned int gmt_RET_STAT;  //GSEL Requirement
enum
{
   // Error status
   OK,                     // Function Call OK
   ERR_SEND,               // Error in sending data
   ERR_RCV,             // Error in receiving data
   ERR_TMO,             // Timeout error
   ERR_PARAM,              // Error in input parameters
   ERR_READ,               // Error in reading
   ERR_WRITE,              // Error in writing
   ERR_COMMAND,            // API command failed
   ERR_POINTER,            // Pointer not intialized
   ERR_OVERFLOW,           // Input parameter vlaues over limit
   ERR_MEMORY,             // Out of SDRAM memory
   ERR_NO_DATA,            // IR FIFO queue is empty
   ERR_BUSY,               // Device busy
   ERR_LIMIT,						// Reach limit of position
   ERR_NO_INIT,					// Never run initial sequence
   ERR_NOT_ACTIVE,    				//
   ERR_OSD_INVALID_TILESET,   // Invalid tileset
   ERR_OSD_INVALID_TILE,      // Invalid tile number
   ERR_OSD_DISABLED_TILE,     // tile is disabled, can't draw the control
   ERR_OSD_INVALID_SIZE,      // Invalid control size
   ERR_OSD_INVALID_BPP,    // Invalid color depth
   ERR_OSD_INVALID_HILITE,    // HiLite window exceeds max
   ERR_OSD_INVALID_FONT_ID,   // Invalid font id
   ERR_OSD_TOOBIG_STRING,     // Temporary buffer size is too small for rendering
   ERR_ADC_CALIBRATE,         // Error in ADC calibration
   ERR_BIT_NOT_SET,        // Status Bit not set
   ERR_BIT_NOT_CLEAR,				// Status Bit not clear
   ERR_FSB_WR_SDRAM_FIFO_ALMOST_EMPTY,
   ERR_FSB_DATA_EXPANDER_RDY,
   ERR_FSB_WR_XFER_IN_PROGRESS,
   ERR_FSB_WR_SDRAM_FIFO_FULL,
   ERR_FSB_RD_SDRAM_FIFO_FULL,
   ERR_FSB_RD_SDRAM_FIFO_EMPTY,
   ERR_FSB_RD_XFER_IN_PROGRESS,
   FAIL,
   ERR_RFU = ERR_FSB_RD_XFER_IN_PROGRESS + 20,  //reserved for future use
   LAST_IROM_DEFINED_RET_STAT = ERR_RFU
};

//*************************************************************************************************
// DMA DRQ selection table
//*************************************************************************************************
typedef enum
{
   DMA_FS_BRIDGE_WRITE	   = 0,
   DMA_FS_BRIDGE_READ	   = 1,
   DMA_DDC2BI_RD		   = 2,
   DMA_DDC2BI_WR		   = 3,
   DMA_LOADER_VLUT		   = 4,
   DMA_LOADER_OSD		   = 5,
   DMA_LOADER_MVF		   = 6,
   DMA_LOADER_PVF		   = 7,
   DMA_LOADER_MHF		   = 8,
   DMA_LOADER_PHF		   = 9,
   DMA_LOADER_ACC		   =10,
   DMA_LOADER_LPM		   =11,
   DMA_I2C_MASTER		   =12,
   DMA_SSLAVE_RD		   =13,
   DMA_SSLAVE_WR		   =14,
   DMA_FORCE_MODE		   =15,
   DMA_LOADER_IMP_VLUT	   =16
} DMA_REQ_CLIENT;

//*************************************************************************************************
// globalData.c
//*************************************************************************************************
//DO NOT change the sequence!!
typedef enum
{
   I2C_SLAVE1_CH,
   I2C_SLAVE2_CH,
   I2C_SLAVE3_CH,
   I2C_SLAVE4_CH,
   I2C_SLAVE_NUM_CH
}SLAVE_CH;

typedef void (far* FPT_ISR)(void);

typedef void (far* I2C_SLAVE_FPT_ISR)(SLAVE_CH);
extern I2C_SLAVE_FPT_ISR gFp_IsrI2cSlave;
extern FPT_ISR gFp_IsrDdc2b;

extern FPT_ISR gFp_IsrSSlave;
extern FPT_ISR gFp_IsrIr;
extern FPT_ISR gFp_IsrFsbWrDone;
extern FPT_ISR gFp_IsrRsReady;
extern FPT_ISR gFp_IsrIrq1Man;
extern FPT_ISR gFp_IsrIrq2Man;
extern FPT_ISR gFp_IsrIrq3Man;
extern FPT_ISR gFp_IsrIrq5Man;

// The variable holds the number of bit that to shift for the CS register during the
// the calculation of the absolute address for 20-bit or 24-bit address mode.  Its
// value is 4 for 20-bit addresss mode or 8 for 24-bit bit address mode.
// The vairable is initialized in gm_RamInit and must not be alternated afterwards.
extern BYTE B_SegBit2Shift; // number of bit to shift for CS
extern WORD W_SegEndExtRom; // external rom end segment
extern WORD CSsave;                 // used by gm_Func to seave code segment
extern WORD IPsave;                 // used by gm_Func to save data segment
extern WORD BPsave;                 // used by gm_Func to save BP register

//*************************************************************************************************
// GM.c
//*************************************************************************************************
// The following definitions define the register values to configure the clock generator

typedef enum
{
   RCLKSEL_432M,
   RCLKSEL_378M,
   RCLKSEL_270M,
   RCLKSEL_END,
}gmt_RCLK_FREQ_SEL;

typedef enum
{
   FSCLK_460M,
   FSCLK_400M,
   FSCLK_300M,
   FSCLK_250M,
   FSCLK_200M,
   FSCLK_176_9M,
   FSCLK_135M,
   FSCLK_100M,
   FSCLK_27M,
   FSCLK_DEF_END,
   FSCLK_150M, // For OCM only
}gmt_FS432Clk;

void huge gm_McuInit(void);//jmp_tbl
void huge gm_ProgramLpmClkandPbus(void);
void huge gm_ProgramRCLKandOCLK(gmt_FS432Clk OcmClkSel, gmt_RCLK_FREQ_SEL RclkSel);//jmp_tbl
void huge gm_ProgramRCLKandOCLKandTimer(gmt_FS432Clk OcmClkSel, gmt_RCLK_FREQ_SEL RclkSel, DWORD OcmClk);//jmp_tbl
void far* far _fmemcpy(void far* pdst, const void far* psrc, WORD size);//jmp_tbl
void far* far _fmemset(void far*, int, unsigned);//jmp_tbl
void far gm_func(void (near *funcptr)(void));//jmp_tbl

//*************************************************************************************************
// Main.c
//*************************************************************************************************
void huge main_c(void);//jmp_tbl

//*************************************************************************************************
// D_TIMER.C
//*************************************************************************************************
// 5 4 3 2 1 0  byte   System time in ms, maximum time ~ 16years
// |_|_|_|_|_|________BYTE b[5]
//     |_|_|_|________DWORD d, ms
//         |_|________WORD w, ms
typedef union
{
   BYTE b[5];
   WORD w;
   DWORD d;
} gmt_STime;

// 7 6 5 4 3 2 1 0 byte Precise system time in ms and clk (Cortez Premium: 35 clk = 1mks)
// | | | | | | |_|________WORD clk, clk
// |_|_|_|_|_|________BYTE ms.b[5]
//     |_|_|_|________DWORD ms.d, ms
//         |_|________WORD ms.wt, ms
#pragma pack(push)
#pragma pack(1)
typedef struct
{
   WORD clk;
   gmt_STime ms;
} gmt_STimeEx;
#pragma pack(pop)

typedef enum
{
   TIMER_OK = 0,  // Timer running
   TIMER_TMO   = 1,  // Timer timeout
   TIMER_STOP = 2,     // Timer stop (compatibility only)
} gmt_TimerStatus;

// Define all application timers
enum                             //
{
   START_1ms,                 // ***** 1ms timer start
   DELAY_1ms_TMR=START_1ms,      // (0) Delay timer
   WORK1_1MS_TMR,             // (1) General working timer
   END_1ms,                // ***** 1ms timer end
   //
   START_10ms=END_1ms,           // ***** 10ms timer start
   DELAY_10ms_TMR=START_10ms,    // (2) Delay timer
   UART_10ms_TMR,             // (3) UART communication timer
   WORK1_10MS_TMR,               // (4) General working timer
   END_10ms,                  // ***** 10ms timer end
   //
   START_100ms=END_10ms,         // ***** 100ms timer start
   DELAY_100ms_TMR=START_100ms,  // (5) Delay timer
   SYSTEM_WATCHDOG_TMR,       // (6) System WatchDog - reserved
   END_100ms,                 // ***** 100ms timer end
   MAX_TMR=END_100ms          // Total Max number of timer (7)
};

DWORD huge gm_GetSystemTime(void);//jmp_tbl
WORD huge gm_GetSystemTimeW(void);//jmp_tbl
void huge gm_GetSystemTimePrecise(gmt_STimeEx fdata * const timex);//jmp_tbl
void huge gm_TimerInit   (DWORD D_CpuClock);//jmp_tbl
void huge gm_TimerCpuClock(DWORD D_CpuClock);//jmp_tbl
void huge gm_TimerStartEx(gmt_STimeEx fdata* const ptimer, DWORD ms, BYTE ten_mks);//jmp_tbl
void far gm_TimerStopEx(gmt_STimeEx fdata* const ptimer);//jmp_tbl
gmt_TimerStatus huge gm_TimerCheckEx(gmt_STimeEx const fdata* const ptimer, DWORD fdata* const pms, BYTE fdata* const ptmks);//jmp_tbl
void huge gm_StartHwResponseTimer(WORD ms);//jmp_tbl
#define gmTimerStartWd(ms) W_HwResponseTimer = ms
gmt_TimerStatus huge gm_CheckHwResponseTimer(void);//jmp_tbl
#define gmTimeOutWd (W_HwResponseTimer==0)
void huge gm_TimerIsr(void); //jmp_tbl
void huge gm_Delay(DWORD ms, BYTE ten_mks);//jmp_tbl

void huge gm_UserTimerInitEx(WORD fdata* Wp_UserTimerP, BYTE B_Num1msTimers, BYTE B_Num10msTimers, BYTE B_Num100msTimers, BYTE B_Num1STimers);//jmp_tbl
void huge gm_UserTimerInit(WORD near* Wp_UserTimerP, BYTE B_Num1msTimers, BYTE B_Num10msTimers, BYTE B_Num100msTimers, BYTE B_Num1STimers);//jmp_tbl
void huge gm_GselTimerInitEx(WORD fdata* Wp_GselTimerP, BYTE B_Num1msTimers, BYTE B_Num10msTimers, BYTE B_Num100msTimers, BYTE B_Num1STimers);//jmp_tbl
void far gm_GselTimerInit(WORD near* Wp_GselTimerP, BYTE B_Num1msTimers, BYTE B_Num10msTimers, BYTE B_Num100msTimers, BYTE B_Num1STimers);//jmp_tbl
void huge gm_TimerStart(BYTE B_Timer, WORD W_Value); //jmp_tbl
gmt_TimerStatus huge gm_TimerCheck(BYTE B_Timer); //jmp_tbl
void huge gm_TimerStop   (BYTE B_Timer); //jmp_tbl
WORD huge gm_TimerRead   (BYTE B_Timer); //jmp_tbl
void far gm_Delay1ms(WORD value); //jmp_tbl
void far gm_Delay10ms(WORD value); //jmp_tbl
void far gm_Delay100ms(WORD value); //jmp_tbl

extern BYTE gB_Count10ms;
extern BYTE gB_Count100ms;
extern BYTE gB_Count1s;
extern WORD gWa_TmrBuf[MAX_TMR];
extern BYTE gB_User1msTimers;
extern BYTE gB_User10msTimers;
extern BYTE gB_User100msTimers;
extern BYTE gB_User1STimers;
extern BYTE gB_NumUserTimers;
extern WORD fdata* gWp_UserTimerp;
extern BYTE gB_Gsel1msTimers;
extern BYTE gB_Gsel10msTimers;
extern BYTE gB_Gsel100msTimers;
extern BYTE gB_Gsel1STimers;
extern BYTE gB_NumGselTimers;
extern WORD fdata* gWp_GselTimerp;
extern BYTE gB_MaxTimers;

extern volatile gmt_STime g_SystemTime;
extern volatile WORD W_HwResponseTimer;
extern DWORD gDw_SystemIdleTime; //idle time counter
extern void far (*gm_IdleFunc)(DWORD);
extern void far (*gm_1msFunc)(void);

//*************************************************************************************************
// D_UART.C
//*************************************************************************************************
#define SIO_TxBufLen 255
#define SIO_RxBufLen 256

typedef enum
{
   UART0 = 0,
   UART1 = 1,
   UART_NUMBER
} TUartChannel;

typedef enum
{
   READ_PKT_OK,
   READ_PKT_NO_MSG,
   READ_PKT_WRONG_CS,
} TReadPkt;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   gmt_STimeEx timer;
   volatile WORD rcvCount;
   volatile BYTE trxCount;
   volatile BYTE trxCS;
   WORD trxTimeout;
   WORD rcvTimeout;
   BYTE rcvBuf[SIO_RxBufLen];
   BYTE trxBuf[SIO_TxBufLen];
} TUartData;
#pragma pack(pop)

extern TUartData far* gmp_uart[UART_NUMBER];

gmt_RET_STAT huge gm_UartTrxChar(BYTE B_Data);//jmp_tbl
gmt_RET_STAT huge gm_Uart1TrxChar(BYTE B_Data);//jmp_tbl
BOOL huge gm_UartIsInTrx(TUartChannel ch); //jmp_tbl
void huge gm_UartClock(TUartChannel ch, DWORD D_CpuClock, DWORD D_BaudRate); //jmp_tbl
void huge gm_UartInit(TUartChannel ch, DWORD D_CpuClock, DWORD D_BaudRate, TUartData fdata* pUart); //jmp_tbl
TReadPkt huge gm_UartReadCmdPkt(TUartChannel ch, BYTE far*const pBuf); //jmp_tbl
void huge gm_UartWriteCmdPkt(TUartChannel ch, void fdata* pBuffer); //jmp_tbl
WORD huge gm_Uart_GetRcvCnt(TUartChannel ch); //jmp_tbl

//*************************************************************************************************
// DDC2BI.C
//*************************************************************************************************
#define DDC2B_SRC_ADDRESS               0x51
#define DDC2Bi_CONTROL_STATUS_FLAG      BIT7
#define DDC2BI_NULL_MSG_LENGTH          0x80
#define INIT_NULL_MSG_CS               (0x80 ^ 0x50)
#define NULL_MSG_SECOND_BYTE    		    0x80 //Add by ChengLin 20130514
#define DDC2BI_MAX_MSG_LENGTH           0x7F
#define DDC2BI_BUF_SIZE (0x7F+4) //buffer size 

#define DDC2BI_ADDR_ID        			0x6E


// used for DDC2Bi channel selection
typedef enum
{
   DDC2BI_CHANNEL0 = 0,
   DDC2BI_CHANNEL1 = 1,
} DDC2BI_CHANNEL;

// DDC2Bi Protocol Status bits definitions and masks
// 7 6 5 4 3 2_1_0__keeps first error code
// |_|_|_|_|________errors counter
typedef enum
{
   DDC2BI_ERROR_CRC = 1,
   DDC2BI_ERROR_OVERRUN = 4, //historically
   DDC2BI_ERROR_MASK  = 7,
   DDC2BI_ERROR_COUNTER = BIT3
} DDC2BI_ERRORS;

typedef enum
{
   DDC2BI_RCV_STATE_STOP_OCCURED,
   DDC2BI_RCV_STATE_MSG_RECEIVE,
   DDC2BI_RCV_STATE_PARSER_READY,
} DDC2BI_RCV_STATES;

typedef enum
{
   // for transmission
   DDC2BI_TR_STATE_RESPONSE_READY,   // prepared in foreground
   DDC2BI_TR_STATE_CONTINUE_TRX,    //associated
   DDC2BI_TR_STATE_NULL_MSG,
} DDC2BI_TR_STATES;

typedef enum
{
   DDC2BI_STATE_RCV_STOP_OCCURED,
   DDC2BI_STATE_READY,
   DDC2BI_STATE_CONSECUTIVE_TRANSMISSION
}DDC2BI_STATES;

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE devId;
   DDC2BI_CHANNEL channel;
   BYTE trCount;
   BYTE rcvCount;
   BYTE trLength;
   volatile DDC2BI_STATES state;
   BYTE trxTimeout; //transmission timeout in ms;
   BYTE buf[DDC2BI_BUF_SIZE];
   BYTE turnOff;//temporal to have ability to turn off new hardware:  BIT0=1 stop-condition hardware reset WFIFO is off, BIT1=1 = divider of timeout is off (DDC2B_ADDR_CS_TO:15)
} TDdc2bi;
#pragma pack(pop)

//***************
extern TDdc2bi fdata* gmp_Ddc2bi;
extern WORD gDDC2BiTrCount;
extern WORD gDDC2BiRcvCount;
extern BYTE gB_txLength;
extern DDC2BI_CHANNEL gDDC2BiChannel;
extern DDC2BI_TR_STATES gDDC2BiTrState; //= DDC2BI_TR_STATE_NULL_MSG;
extern volatile DDC2BI_RCV_STATES gDDC2BiRcvState; // = DDC2BI_RCV_STATE_PARSER_READY;
extern BYTE fdata* ddc2biPBuf;
extern BYTE gB_DDC2BiDevId; //= 0x6E;
extern BYTE gB_GprobeCmdSet; //= 0xC2;
extern WORD gW_ManufacturerID; //= 0;

BYTE huge gm_DDC2Bi_GetTranslCmd(void);//jmp_tbl
void huge gm_DDC2Bi_SetTranslCmd(BYTE);//jmp_tbl
WORD huge gm_DDC2Bi_GetManufacturerID(void);//jmp_tbl
void huge gm_DDC2Bi_SetManufacturerID(WORD);//jmp_tbl
BOOL huge gm_DDC2Bi_CheckCS(BYTE B_DevID, BYTE B_Len, BYTE *Bp_Buff);//jmp_tbl

//***************
#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE devId;
   BYTE trCount;
   BYTE rcvCount;
   BYTE trLength;
   volatile DDC2BI_STATES state;
   BYTE trxTimeout; //transmission timeout in ms;
   BYTE buf[DDC2BI_BUF_SIZE];
   BYTE turnOff;//temporal to have ability to turn off new hardware:  BIT0=1 stop-condition hardware reset WFIFO is off, BIT1=1 = divider of timeout is off (DDC2B_ADDR_CS_TO:15)
} TSSlave;
#pragma pack(pop)

extern TSSlave fdata* gmp_SSlave;

void huge gm_SSlave_Init(TSSlave fdata* pBuf); //jmp_tbl
void huge gm_SSlave_Reinit(void); //jmp_tbl
BOOL huge gm_SSlaveWrite(const void far* pBuf, BYTE size); //jmp_tbl
TReadPkt huge gm_SSlaveRead(BYTE far* pBuf); //jmp_tbl

//*************************************************************************************************
// DEBUG.C
//*************************************************************************************************
#define ID_STRING_LEN	8	//with null-terminated

// commands delegated  to the external rom code handle
typedef enum
{
   NO_MSG_PARSED,
   MSG_PARSED,
   UNKNOWN_MSG_CMD
} PARSER_STATUS_t;

// possible parameters for ParseCommand() function
#pragma option -b.
typedef enum
{
   SERIAL_USE_UART0,
   SERIAL_USE_UART1,
   DDC2BI_USE,
   SSLAVE_USE,
   I2C_SLAVE1_USE,
   I2C_SLAVE2_USE,
   I2C_SLAVE3_USE,
   I2C_SLAVE4_USE,
   AUX_C0_USE,
   AUX_C2_USE
} HANDLER_USE_t;
#pragma option -b-

#pragma pack(push)
#pragma pack(1)
typedef struct // command packet to read gm register
{
   BYTE length; // length of packet
   BYTE cmd;
   BYTE addr_hi;
   BYTE addr_lo;
}ReadBrCmd_t;

typedef struct // command packet to extended read gm register
{
   BYTE length; // length of packet
   BYTE cmd;
   BYTE addr_3;
   BYTE addr_2;
   BYTE addr_1;
   BYTE addr_0;
}ReadBr2Cmd_t;

typedef struct // response packet for read gm command
{
   BYTE    length;                             // length of this response packet
   BYTE    cmd;                                // response packet type
   BYTE    addr_hi;                            // address of register read
   BYTE    addr_lo;
   BYTE    value_hi;                           // value read from register
   BYTE    value_lo;
}RdBrCmdPkt_t;

typedef struct // response packet for read gm command
{
   BYTE    length;                             // length of this response packet
   BYTE    cmd;                                // response packet type
   BYTE    addr_3;                            // address of register read
   BYTE    addr_2;
   BYTE    addr_1;                            // address of register read
   BYTE    addr_0;
   BYTE    value_3;                           // value read from register
   BYTE    value_2;
   BYTE    value_1;                           // value read from register
   BYTE    value_0;
}RdBr2CmdPkt_t;

typedef struct // command packet to write gm register
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_hi;
   BYTE    addr_lo;
   BYTE    value_hi;
   BYTE    value_lo;
} WriteBrCmd_t;

typedef struct // command packet to write gm register
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_3;
   BYTE    addr_2;
   BYTE    addr_1;
   BYTE    addr_0;
   BYTE    value_3;
   BYTE    value_2;
   BYTE    value_1;
   BYTE    value_0;
} WriteBr2Cmd_t;

typedef struct // command packet for burst write to RAM
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_hi;
   BYTE    addr_lo;
   BYTE    value[SIO_RxBufLen-6];
}BurstWrRAMCmd_t;

typedef struct // command packet for burst write to RAM
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_3;
   BYTE    addr_2;
   BYTE    addr_1;
   BYTE    addr_0;
   BYTE    value[SIO_RxBufLen-7];
}BurstWrRAM2Cmd_t;

typedef struct // command packet for burst write to RAM from last address
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    value[SIO_RxBufLen-4];
}BurstWrNextRAMCmd_t;

#pragma option -b.
typedef enum // do not change this enumaration !
{
   EXTERNAL_ROM_MODE = 0,
   INTERNAL_ROM_MODE,
   ISP_RAM_MODE,
} MODE_STATUS_t;
#pragma option -b-

typedef struct // command packet for RAM patch execute
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_hi;
   BYTE    addr_lo;
} *pRamExecuteCmd_t;

typedef struct // command packet for RAM patch execute
{
   BYTE    length; // length of packet
   BYTE    cmd;
   BYTE    addr_3;
   BYTE    addr_2;
   BYTE    addr_1;
   BYTE    addr_0;
} RamExecute2Cmd_t;

typedef struct // command packet to read reset request
{
   BYTE length; // length of packet
   BYTE cmd;
   BYTE operation_opcode;
   BYTE chk;
}ResetCmd_t;

typedef struct // command packet to Get Info
{
   BYTE B_Length; // length of packet
   BYTE B_Cmd;
   BYTE B_Type;
   BYTE B_Chk;
}GetInfoCmd_t;

typedef struct // response packet for get info, type=getversion
{
   BYTE  B_Length;                        // length of this response packet
   BYTE  B_Cmd;                        // response packet type
   BYTE  B_FamilyID_Hi;
   BYTE  B_FamilyID_Lo;
   BYTE    B_ChipID_Hi;
   BYTE    B_ChipID_Lo;
   BYTE  B_IROM_Major;
   BYTE  B_IROM_Minor;
   BYTE  B_IROM_Build_Hi;
   BYTE  B_IROM_Build_Lo;
   BYTE  B_FW_Major;
   BYTE  B_FW_Minor;
   BYTE  B_FW_Build_Hi;
   BYTE  B_FW_Build_Lo;
   BYTE  B_chk;                        // checksum of packet
}GetVersionCmd_t;

typedef struct
{
   BYTE    length;                             // Packet length
   BYTE    cmd;                                // Command
   BYTE    addr_3;
   BYTE    addr_2;
   BYTE    addr_1;
   BYTE    addr_0;
   BYTE    count_3;
   BYTE    count_2;
   BYTE    count_1;
   BYTE    count_0;
} RamReadCmd_t;

typedef struct
{
   BYTE    length;                             // Packet length
   BYTE    cmd;                                // Command
   BYTE    value[SIO_RxBufLen-3];
} RamReadResp_t;

typedef struct // response packet for read gm command
{
   BYTE    length;                             // length of this response packet
   BYTE    cmd;                                // response packet type
   BYTE    index;                              // Index of device ID (0-255)
}DevIdCmd_t;

typedef struct // response packet for read gm command
{
   BYTE    length;                             // length of this response packet
   BYTE    cmd;                                // response packet type
   char    idstring[ID_STRING_LEN];            // Index of device ID (0-255)
}DevIdResp_t;

#pragma pack(pop)

//DON'T CHANGE THE SEQUENCE!!
typedef enum
{
   DEVID_CHIP_ID,
   DEVID_VENDOR_ID,
   DEVID_PRODUCT_ID,
   DEVID_FLASH_MAUFACTURE_ID,
   DEVID_FLASH_DEVICE_ID,
   DEVID_IROM_MAJOR_VERSION,
   DEVID_IROM_MINOR_VERSION,
   DEVID_FW_MAJOR_VERSION,
   DEVID_FW_MINOR_VERSION,
} gmt_DeviceIdIndex;

typedef enum
{
   CMD_gmRead                = 0x06, // read  host reg
   CMD_gmWrite                = 0x07, // write host reg
   //CMD_GetVersion        = 0x08, // get firmware version
   CMD_TurnOffSwitcher        = 0x09, // set firmware in DEBUG  mode
   CMD_TurnOnSwitcher        = 0x0A, // set firmware in NORMAL mode
   CMD_PRINTIT                = 0x0D, // print command
   //CMD_I2C_Read            = 0x0E, // read I2C
   //CMD_I2C_Write            = 0x0F, // write I2C
   //CMD_FastWrFlash        = 0x10,    //
   //CMD_SetParam            = 0x11, // custom command
   CMD_AppsTest            = 0x12, // custom command
   CMD_BurstRAMWr            = 0x13, // burst write RAM buffer
   CMD_BurstNextRAMWr        = 0x14, // burst write RAM buffer from last address
   CMD_WrNvramBlock        = 0x15, // write NVRAM from RAM buffer
   CMD_WrNvram            = 0x16, // write NVRAM from data packet
   CMD_RdNvram            = 0x17, // read back NVRAM
   //CMD_BurstRegWr        = 0x18, // burst reg write
   //CMD_FlashErase        = 0x19, // erase FLASH sector
   //CMD_FlashWrite        = 0x1A, // write FLASH from RAM buffer
   //CMD_FlashRead            = 0x1B, // read  FLASH
   //CMD_FlashCRC            = 0x1C, // get FLASH CRC
   CMD_RamExecute            = 0x1D, // execute code from RAM
   CMD_Reset                = 0x20, // force reset
   CMD_DeviceId                = 0x30, // get all related device IDs
   CMD_GetInfo                = 0x40, // get info record - get versiom type supported
   CMD_VarLenRegRead        = 0x46,//variable length register (memory) read
   CMD_VarLenRegWrite    = 0x47,//variable length register (memory) write
   CMD_gmRead2                = 0x50, // read host reg extended (DWORD address)
   CMD_gmWrite2            = 0x51, // write host reg extended (DWORD address)
   CMD_RAMRead2            = 0x52, // burst read RAM buffer (DWORD address)
   CMD_RAMWrite2            = 0x53, // burst write RAM buffer (DWORD address)
   CMD_RamExecute2            = 0x54, // execute code from RAM (DWORD address)
} gmt_GProbeCommandCode;

typedef enum
{
   GProbe_gmRead        = BIT0,
   GProbe_gmWrite       = BIT1,
   GProbe_TurnOffSwitcher  = BIT2,
   GProbe_TurnOnSwitcher   = BIT3,
   GProbe_VarLenRegRead = BIT4,
   GProbe_VarLenRegWrite = BIT5,
   GProbe_BurstRAMWr    = BIT6,
   GProbe_BurstNextRAMWr   = BIT7,
   GProbe_RamExecute    = BIT8,
   GProbe_Reset         = BIT9,
   GProbe_GetInfo       = BIT10,
   GProbe_gmRead2       = BIT11,
   GProbe_gmWrite2         = BIT12,
   GProbe_RAMRead2         = BIT13,
   GProbe_RAMWrite2     = BIT14,
   GProbe_RamExecute2      = (int)BIT15
} gmt_GProbeFlag;

// get info command codes
typedef enum
{
   GetVersion,
   GetSupportedFunctions,
   GetRegInfo,
   GetMemInfo
} gmt_GetInfoEnum;

typedef void (far* FPT_Reset0)(WORD);
typedef BYTE far (* EXTENDED_COMM_PARSER_TYPE)(HANDLER_USE_t B_whichHandler, void far* pBuf);
typedef struct //just to save space in IRAM other 14 bit could be used for something else.
{
   WORD PrintPort : 2; 
} TConfig;

extern TConfig g_Config;
extern BYTE gmvb_DebugOnly;
extern BYTE fdata* gDebugCmdPtr;
extern WORD gmvw_GProbeFlag;
extern BYTE far* gBurstPtr;
extern MODE_STATUS_t B_ModeStatus;
extern FPT_Reset0 gFp_Reset0;
extern const far BYTE AckPkt[];
extern const far BYTE NakPkt[];

typedef enum
{
   AUX_CH_C0 = 0,
   AUX_CH_C2,
   AUX_CH_END,
}AUX_CH;

MODE_STATUS_t huge gm_GetRunModeStatus(void);//jmp_tbl
void huge gm_SetRunModeStatus (MODE_STATUS_t NewStatus);//jmp_tbl
BOOL huge gm_I2CSlaveWriteCmdPkt(void far* pBuf, SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_WriteCmdPkt(void far* pBuf,  WORD Handler);//jmp_tbl
void huge gm_SendAckOrNack(BYTE B_Ack, BYTE B_whichHandler, void far* Bp_MessageBuff);//jmp_tbl
BOOL huge gm_SSlaveWriteCmdPkt(void far* pBuf);//jmp_tbl
PARSER_STATUS_t huge gm_SSlave_Handler(void);//jmp_tbl
BOOL huge gm_AuxRxWriteCmdPkt(void far* pBuf,AUX_CH B_AuxCh);//jmp_tbl
PARSER_STATUS_t huge gm_AuxRxHandler(AUX_CH B_AuxCh);//jmp_tbl
PARSER_STATUS_t huge gm_GprobeCmdParser(HANDLER_USE_t B_whichHandler, void far* pBuf);//jmp_tbl
void huge gm_SetExtendedCommParser(EXTENDED_COMM_PARSER_TYPE Fp_NewFunc);//jmp_tbl
PARSER_STATUS_t huge gm_UartHandler(TUartChannel ch); //jmp_tbl
void huge gm_SetPrintPort(HANDLER_USE_t Handler);//jmp_tbl
void huge gm_ByteToHexChars(BYTE B_Num, char *Bp_String);//jmp_tbl
void huge gm_Print(const far char *Bp_String, ...);//jmp_tbl

//*************************************************************************************************
// I2CSLAVE.C
//*************************************************************************************************
#define  I2C_ADDRESS_BITS        0xFE
#define  ALL_I2C_ADDRESSES       0xFE
#define  I2C_DHA_BUFFER_SIZE     5

#define  MAX_FUNC_NUM            10
#define  I2C_SLAVE_BUFFER_SIZE   132
#define  I2C_SLAVE_RD_LEN        1

typedef BYTE far (* I2C_SLAVE_HANDLER_TYPE)(SLAVE_CH B_SlaveCh);
typedef BOOL far (* I2C_COMM_HANDLER_TYPE)(BYTE B_Channel, BYTE B_DevID, BYTE *Bp_Buff);

typedef struct
{
   BYTE     B_Stop;
   WORD     TxPtr;
   WORD     RxPtr;
   BYTE     State;
   BYTE     HandlerState;
   BYTE     B_DevID;
   BYTE     B_ReplyReady;
   WORD     DevAddr;
   WORD     W_TrxCount;
   WORD     W_RcvCount;
   WORD     W_AbsRcvCount;
   BYTE     Buffer[I2C_SLAVE_BUFFER_SIZE];
   BYTE     B_RdLen;
   BYTE     B_NullMsgPtr;
   BYTE     B_ActiveFlag;
   BYTE     Bp_CorrDevIDArray[MAX_FUNC_NUM];
   I2C_SLAVE_HANDLER_TYPE  Fp_CommHandlerArray[MAX_FUNC_NUM];
   BYTE     B_IsDDCPacket;
   BYTE     B_DDCSendStop;
   BYTE     B_DDCLength;
}I2C_SLAVE_TYPE;

extern I2C_SLAVE_TYPE* Sp_I2CSlavePtrGlobal[I2C_SLAVE_NUM_CH];
extern BYTE far(* I2CSlaveExtHandlerFuncPtr)(SLAVE_CH B_SlaveCh);
extern I2C_COMM_HANDLER_TYPE Fp_CorrespondingFuncPtr;

extern BYTE B_DDCToAuxStatus;
extern BYTE B_SlvStatus[I2C_SLAVE_NUM_CH];
extern BYTE B_SlvStatus2[I2C_SLAVE_NUM_CH];
extern I2C_SLAVE_TYPE I2CSlaveTable[I2C_SLAVE_NUM_CH];
extern BYTE gB_Ddc2Bi_I2C_Status[I2C_SLAVE_NUM_CH];
extern volatile BYTE gB_I2cSlaveSel;//default select I2C_SLAVE4 (VGA), used for select specific I2C_SLAVE register
extern BYTE B_DMAEn;
extern SLAVE_CH gI2CSlaveChannel;
extern BYTE volatile B_DevIdReady;


void huge gm_I2CSlaveInitAll (void);//jmp_tbl
void huge gm_I2CSlaveInit (BYTE* Bp_StartAddr, BYTE B_AuxRdLen, SLAVE_CH B_SlaveCh);//jmp_tbl
I2C_SLAVE_TYPE* huge gm_I2CSlaveGetAddrBuffer(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveSetChannel(SLAVE_CH B_SlaveCh);//jmp_tbl
SLAVE_CH huge gm_I2CSlaveGetChannel(void);//jmp_tbl
void huge gm_I2CSlaveAddI2CAddress(BYTE B_I2CAddr, SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveRemoveI2CAddress(BYTE B_I2CAddr, SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveWriteToFifo(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveReadFromFifo(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveReadFromFifo_new(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveHandler(void);//jmp_tbl
void huge gm_I2CSlaveChannelHandler(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveIsr(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveIsrNew(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveIsrOld(SLAVE_CH B_SlaveCh);//jmp_tbl
void huge gm_I2CSlaveSetReply(BYTE B_Count, SLAVE_CH B_SlaveCh);//jmp_tbl
//void huge gm_I2CSendAckOrNack(BYTE B_Ack);
//void huge gm_I2CCreateAckNackMsg(BYTE B_Ack, BYTE B_DevId, BYTE *Bp_Buff);
void huge gm_I2CSlaveSetExtHandler(BYTE far(* FuncPtr)(SLAVE_CH B_SlaveCh));//jmp_tbl
BYTE huge gm_I2CSlaveExecCommHandler(SLAVE_CH B_SlaveCh);//jmp_tbl
BYTE huge gm_I2CSlaveDefaultHandler(SLAVE_CH B_SlaveCh);//jmp_tbl
BYTE huge gm_I2CSlaveCreateNullMsg(BYTE B_Stop, BYTE B_DevID, SLAVE_CH B_SlaveCh);//jmp_tbl
BOOL huge gm_I2CSlaveCheckCS(BYTE B_DevID, BYTE B_Len, BYTE *Bp_Buff);//jmp_tbl
BYTE* huge gm_I2CSlaveGetAddrMsgBuffer(SLAVE_CH B_SlaveCh);//jmp_tbl
BOOL huge gm_I2CSlaveAddCommHandler(I2C_SLAVE_HANDLER_TYPE Fp_NewCommHandler, BYTE B_CorrDevID, SLAVE_CH B_SlaveCh);//jmp_tbl
BYTE huge gm_I2CSlaveGProbeHandler(SLAVE_CH B_SlaveCh);//jmp_tbl

//*************************************************************************************************
// I2CSLAVE_HL.C
//*************************************************************************************************
#define MAX_DEV_IDS 3

typedef struct
{
   BYTE                   B_MaxFuncNum;
   BYTE                   B_CurFuncNum;
   BYTE                   Bp_CorrDevIDArray[MAX_DEV_IDS];
   I2C_COMM_HANDLER_TYPE  Fp_CommHandlerArray[MAX_DEV_IDS];
}I2C_SLAVE_HL_TYPE;

extern I2C_SLAVE_HL_TYPE    I2CSlaveHLModuleData[I2C_SLAVE_NUM_CH];

I2C_COMM_HANDLER_TYPE huge gm_I2CSlaveHLFindCommHandler(BYTE B_DevID, SLAVE_CH B_SlaveCh);//jmp_tbl
I2C_SLAVE_HL_TYPE* huge gm_I2CSlaveHLGetAddrModuleData(SLAVE_CH B_SlaveCh);//jmp_tbl

//*************************************************************************************************
// D_TABLEACCESS.C
//*************************************************************************************************
#pragma option -b. //turn on option "allocate enumerators as integer"
typedef enum
{
   gmd_TAB_VLUT = 0,
   gmd_TAB_ACC_BIN_SLOPE = 1,
   gmd_TAB_OSDLUT  = 2,
   gmd_TAB_LPM     = 3,
   gmd_TAB_MVF     = 4,
   gmd_TAB_MHF     = 5,
   gmd_TAB_PVF     = 6,
   gmd_TAB_PHF     = 7,
   gmd_TAB_IN_GAMMA_R = 8,
   gmd_TAB_IN_GAMMA_G = 9,
   gmd_TAB_IN_GAMMA_B = 10,
   gmd_TAB_IN_GAMMA_R_STEEP = 11,
   gmd_TAB_IN_GAMMA_G_STEEP = 12,
   gmd_TAB_IN_GAMMA_B_STEEP = 13,
   gmd_TAB_OUT_GAMMA_R = 14,
   gmd_TAB_OUT_GAMMA_G = 15,
   gmd_TAB_OUT_GAMMA_B = 16,
   gmd_TAB_OUT_GAMMA_R_STEEP = 17,
   gmd_TAB_OUT_GAMMA_G_STEEP = 18,
   gmd_TAB_OUT_GAMMA_B_STEEP = 19,
   gmd_TAB_ACC_TEXTURE_BLEND = 20,
   gmd_TAB_ACC_LUT = 21,
   gmd_TAB_ACC_STATIC_SLOPE = 22,
   gmd_TAB_ACC_INPUT_HISTOGRAM = 23,
   gmd_TAB_SIZE,
   gmd_MAX_NUMBER_TABLES = gmd_TAB_SIZE,
} gmt_TAB_TABLE;

typedef enum
{
   gmd_DMA_MODE         = 0,
   gmd_OCM_POLLING_MODE = 1,
   gmd_OCM_STALLING_MODE   = 2,
   gmd_MAX_NUMBER_TABLE_TRANSFER_MODES
} gmt_TAB_TRANSFER_MODE;

typedef enum
{
   gmd_SAFE_MODE        = 0,
   gmd_HIGH_PRIORITY_MODE  = 1,
   gmd_MAX_NUMBER_TABLE_ACCESS_MODES
} gmt_TAB_ACCESS_MODE;

#pragma option -b-        //turn off option "allocate enumerators as integer"

typedef struct
{
   WORD ctrlAddr; //address of crl host register TABLE_ACCESS_BRIDGE_CTRL1 or TABLE_ACCESS_BRIDGE_CTRL2
   WORD ctrlMask; //bit mask of apropriate table inside ctrl register
   WORD ctrlMode[gmd_MAX_NUMBER_TABLE_TRANSFER_MODES];
   WORD ctrlAccs[gmd_MAX_NUMBER_TABLE_ACCESS_MODES];
   WORD status; //apropriate bit in status register
   WORD request; //mask in DMA_REQUEST_CTRL register
   DWORD memAddr; //address in memory space reserved for that table
   WORD tabSize; //size of table
} TTableLoadParams;

extern TTableLoadParams const far TableLoadingParams[gmd_TAB_SIZE];
extern WORD gW_TimeoutTabPoll; //=10000 32 sec;
extern WORD gW_TimeoutDma1; //= 10000; 32 sec;

WORD huge gm_LoadSRAMBlockEx(gmt_TAB_TABLE Table,  gmt_TAB_TRANSFER_MODE TransferMode, gmt_TAB_ACCESS_MODE AccessMode, WORD far *PSource,  WORD DestOffset, WORD BlockSize);//jmp_tbl
WORD huge gm_ReadSRAMBlock(gmt_TAB_TABLE Table, gmt_TAB_ACCESS_MODE AccessMode,    WORD far *PDest, WORD DestOffset, WORD BlockSize);//jmp_tbl
BOOL huge gm_SRAMBlockDmaReady(void);//jmp_tbl

//*************************************************************************************************
// MEMORY.C
//*************************************************************************************************
// FS Bridge SDRAM address, Hardware specific
#define WR_SDRAM_DATA 0xF880   //FSBridge WR address
#define RD_SDRAM_DATA 0xF800   //FSBridge RD address

typedef enum // Definition of Data Expand factor, bit 6:3 of WR_SDRAM_DATA_EXPAND_CONTROL
{
   EXPAND_1_TO_1  = (0x0 << 3),  // 0000, 1-bit to 1-bit expansion
   EXPAND_1_TO_2  = (0x1 << 3),  // 0001, 1-bit to 2-bit expansion
   EXPAND_1_TO_4  = (0x2 << 3),  // 0010, 1-bit to 4-bit expansion
   EXPAND_1_TO_8  = (0x3 << 3),  // 0011, 1-bit to 8-bit expansion
   EXPAND_1_TO_16 = (0x4 << 3),  // 0100, 1-bit to 16-bit expansion
   EXPAND_2_TO_2  = (0x5 << 3),  // 0101, 2-bit to 2-bit expansion
   EXPAND_2_TO_4  = (0x6 << 3),  // 0110, 2-bit to 4-bit expansion
   EXPAND_2_TO_8  = (0x7 << 3),  // 0111, 2-bit to 8-bit expansion
   EXPAND_2_TO_16 = (0x8 << 3),  // 1000, 2-bit to 16-bit expansion
   EXPAND_4_TO_4  = (0x9 << 3),  // 1001, 4-bit to 4-bit expansion
   EXPAND_4_TO_8  = (0xA << 3),  // 1010, 4-bit to 8-bit expansion
   EXPAND_4_TO_16 = (0xB << 3),  // 1011, 4-bit to 16-bit expansion
   EXPAND_8_TO_8  = (0xC << 3),  // 1100, 8-bit to 8-bit expansion
   EXPAND_8_TO_16 = (0xD << 3)      // 1101, 8-bit to 16-bit expansion
} gmt_DataExpandFactor;

typedef enum // Auto-fill modes for SDRAM (set value according to WR_SDRAM_CONTROL register)
{
   RGB_AUTOFILL_MODE    = 0 << 3,
   CONSTANT_AUTOFILL_MODE  = 1 << 3,
   YUV_AUTOFILL_MODE    = 2 << 3,
   RMW_MODE          = 3 << 3
} gmt_SdramAutofillMode;

typedef void huge (far* FPT_SdramWriteInit)(DWORD, WORD, WORD, WORD, WORD);
extern FPT_SdramWriteInit gFp_SdramWriteInit; //= gm_SdramWriteInit

typedef void huge (far* FPT_SdramReadInit)(DWORD, WORD, WORD, WORD, WORD);
extern FPT_SdramReadInit gFp_SdramReadInit; //= gm_SdramReadInit;

typedef void huge (far* FPT_DmaSdramWritingInit)(WORD, DWORD);
extern FPT_DmaSdramWritingInit gFp_DmaSdramWritingInit; //= gm_DmaSdramWritingInit;

typedef void huge (far* FPT_DmaSdramReadingInit)(WORD, DWORD);
extern FPT_DmaSdramReadingInit gFp_DmaSdramReadingInit; //= gm_DmaSdramReadingInit;

typedef gmt_RET_STAT huge (far* FPT_DataExpandInit)(WORD far*, gmt_DataExpandFactor);
extern FPT_DataExpandInit gFp_DataExpandInit; //= gm_DataExpandInit

typedef gmt_RET_STAT huge (far* FPT_SdramWrite_HA)(DWORD, WORD, WORD, WORD, WORD, WORD far*, WORD far*, gmt_DataExpandFactor, WORD);
extern FPT_SdramWrite_HA gFp_SdramWrite_HA; //= gm_SdramWrite_HA;

typedef gmt_RET_STAT huge (far* FPT_SdramFill)(gmt_SdramAutofillMode, DWORD, WORD, WORD, WORD, WORD, DWORD);
extern FPT_SdramFill gFp_SdramFill; //= gm_SdramFill;

extern volatile BYTE gDmaChannel0Busy; //= FALSE;
extern volatile BYTE gFSBWriteFifoBusy; //= FALSE;
extern volatile BYTE gFSBReadFifoBusy; //= FALSE;
extern WORD gW_TimeoutDma0; // = 10000 - 10 sec by default
extern WORD gW_TimeoutFsbWr; // = 10000 - 10 sec by default
extern WORD gW_TimeoutFsbRd; // = 10000 - 10 sec by default
extern WORD gW_TimeoutExpander; // = 10000 - 10 sec by default
extern DWORD gmv_TeletextDataSdramStartAddress; //= 0;

void huge gm_Dma0Init(void);//jmp_tbl

void huge gm_ResetFSBWriteFifo(void);//jmp_tbl
void huge gm_ResetFSBReadFifo(void);//jmp_tbl
BYTE huge gm_RequestDma0Ex(void);//jmp_tbl
BYTE huge gm_RequestFSBWriteFifo(void);//jmp_tbl
BYTE huge gm_RequestFSBReadFifo(void);//jmp_tbl
void huge gm_ReleaseFSBWriteFifo(void);//jmp_tbl
void huge gm_ReleaseFSBReadFifo(void);//jmp_tbl
void huge gm_SdramWriteInit(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset);//jmp_tbl
void huge gm_SdramReadInit(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset);//jmp_tbl
void huge gm_DmaSdramWritingInit(WORD W_Size, DWORD D_SrcPointer);//jmp_tbl
void huge gm_DmaSdramReadingInit(WORD W_Size, DWORD D_DestPointer);//jmp_tbl
gmt_RET_STAT huge gm_DataExpandInit(WORD far* Wp_ExpData, gmt_DataExpandFactor W_ExpCtrl);//jmp_tbl
gmt_RET_STAT huge gm_SdramWriteHaEx(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD far* Wp_Data, WORD far* Wp_ExpData, gmt_DataExpandFactor W_ExpCtrl, WORD W_SizeData);//jmp_tbl
gmt_RET_STAT far gm_SdramWrite_HA(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD far *Wp_Data, WORD near*Wp_ExpData, WORD W_ExpCtrl, WORD W_SizeData);//jmp_tbl
gmt_RET_STAT huge gm_SdramReadHaEx (DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD far* Wp_Data);//jmp_tbl
gmt_RET_STAT far gm_SdramRead_HA(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD near*Wp_Data);//jmp_tbl
gmt_RET_STAT huge gm_SdramFillEx(gmt_SdramAutofillMode W_Mode, DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, DWORD D_Data);//jmp_tbl
gmt_RET_STAT far gm_SdramFill(WORD W_Mode, DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, DWORD D_Data); //jmp_tbl
gmt_RET_STAT huge gm_SdramWriteDmaEx(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD far *Wp_Data, WORD far* Wp_ExpData, gmt_DataExpandFactor W_ExpCtrl, WORD W_SizeData, WORD W_Size);//jmp_tbl
gmt_RET_STAT far gm_SdramWrite_DMA(DWORD, WORD, WORD, WORD, WORD, WORD far*, WORD near*, WORD, WORD, WORD);//jmp_tbl
gmt_RET_STAT huge gm_SdramReadDmaEx(DWORD D_Addr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset, WORD far* Wp_Data);//jmp_tbl
gmt_RET_STAT far gm_SdramRead_DMA(DWORD D_Addr, WORD W_Stride, WORD W_Width,  WORD W_Height, WORD W_Offset, WORD near*Wp_Data);//jmp_tbl
gmt_RET_STAT huge gm_SdramBlockCopyEx(DWORD D_DestAddr, DWORD D_SrcAddr, WORD W_Stride, WORD W_Width, WORD W_Height, WORD W_Offset,  WORD far* Wp_ExpData, gmt_DataExpandFactor ExpandFactor);//jmp_tbl
gmt_RET_STAT far gm_SdramBlockCopy(DWORD D_DestAddr, DWORD D_SrcAddr,   WORD W_Stride, WORD W_Width, WORD W_Height,  WORD W_Offset, WORD near* Wp_ExpData, WORD W_ExpCtrl); //jmp_tbl
void huge SetTeletextDataSdramStartAddress(DWORD D_Addr);//jmp_tbl
gmt_RET_STAT huge gm_SdramReadTtxDataEx(DWORD D_Addr, WORD W_Width, WORD far* Wp_Data);//jmp_tbl
void far gm_SdramReadTtxData(DWORD D_Addr, WORD W_Width, WORD near* Wp_Data);//jmp_tbl
gmt_RET_STAT huge gm_SdramWriteTtxDataEx(DWORD D_Addr, WORD W_Width, WORD far* Wp_Data);//jmp_tbl
void far gm_SdramWriteTtxData(DWORD D_Addr, WORD W_Width, WORD near* Wp_Data);//jmp_tbl

//*************************************************************************************************
// Compress.c
//*************************************************************************************************
WORD huge gm_GmcaCycleDecompress(BYTE far *Bp_Src, BYTE near *Bp_Dest, WORD (far *LoadFunc)(BYTE near *Bp_Src, WORD W_Size));//jmp_tbl
WORD huge gm_GmcaCycleDecompressEx(BYTE far* Bp_Src, BYTE fdata* Bp_Dest, WORD W_BufferSize, WORD (far *LoadFunc)(BYTE far* Bp_Src, WORD W_Size));//jmp_tbl

//*************************************************************************************************
// OSD.C
//*************************************************************************************************
#define MAX_OSD_WIDTH 2560

// Tile records in COMMAND SRAM are double buffered. There are two different tilesets: Active and Pending
#define NUMBER_OF_TILESETS 2
// Maximum number of tiles visible at the same moment
#define MAX_NUMBER_OF_TILES 16
// Maximum number of highlight windows which could be simulteniously applied to a tile
#define MAX_NUMBER_OF_HILITE_WINDS 4

#define TILESET_1_ADDRESS (0xC000)
#define TILESET_0_ADDRESS (TILESET_1_ADDRESS + 40 * MAX_NUMBER_OF_TILES)

#define MAX_REG_EXPANSION    16

// flags in W_Flags of ST_RENDERED_TEXT rotation flag
#define ROTATION_BIT    BIT4
// flag of no color map
#define NO_COLOR_MAP_BIT    BIT15

#define OSD_TILE_TRANSPARENCY_BYTE_NUMBER                38
#define OSD_TILE_BLEND_BYTE_NUMBER                        38
#define OSD_TILE_BLINK_BYTE_NUMBER                        38
#define OSD_TILE_TRANS_BEHAVIOR_BYTE_NUMBER                38
#define OSD_TILE_BLINK_BEHAVIOR_BYTE_NUMBER                38

#define OSD_TILE_TRANSPARENCY_BIT_NUMBER                (1 << 3)
#define OSD_TILE_BLEND_BIT_NUMBER                        (1 << 4)
#define OSD_TILE_BLINK_BIT_NUMBER                        (1 << 5)
#define OSD_TILE_TRANS_BEHAVIOR_BIT_NUMBER                (1 << 6)
#define OSD_TILE_BLINK_BEHAVIOR_BIT_NUMBER                (1 << 7)

#define OSD_TILE_HILITE_1_TRANSPARENCY_BYTE_NUMBER        21
#define OSD_TILE_HILITE_2_TRANSPARENCY_BYTE_NUMBER        15
#define OSD_TILE_HILITE_3_TRANSPARENCY_BYTE_NUMBER        9
#define OSD_TILE_HILITE_4_TRANSPARENCY_BYTE_NUMBER        3

#define OSD_TILE_HILITE_1_BLINK_BYTE_NUMBER                23
#define OSD_TILE_HILITE_2_BLINK_BYTE_NUMBER                17
#define OSD_TILE_HILITE_3_BLINK_BYTE_NUMBER                11
#define OSD_TILE_HILITE_4_BLINK_BYTE_NUMBER                5

#define OSD_TILE_HILITE_TRANS_BEHAVIOR_BYTE_NUMBER        25

#define OSD_TILE_HILITE_TRANSPARENCY_BIT_NUMBER            (1 << 7)
#define OSD_TILE_HILITE_BLINK_BIT_NUMBER                (1 << 7)

#define OSD_TILE_HILITE_1_TRANS_BEHAVIOR_BIT_NUMBER        (1 << 7)
#define OSD_TILE_HILITE_2_TRANS_BEHAVIOR_BIT_NUMBER        (1 << 6)
#define OSD_TILE_HILITE_3_TRANS_BEHAVIOR_BIT_NUMBER        (1 << 5)
#define OSD_TILE_HILITE_4_TRANS_BEHAVIOR_BIT_NUMBER        (1 << 4)

#define OSD_TILE_CLEAR_FLAG                                BIT0
#define OSD_TILE_NOT_ENABLE_FLAG                        BIT1
#define OSD_TILE_SAVE_BANDWIDTH_STRIDE_FLAG                BIT2

// Defines a bitmap image header which precede actual compressed pixel data
#define BITMAP_WIDTH_OFFSET      1
#define BITMAP_HEIGHT_OFFSET  3
#define BITMAP_DATA_OFFSET    5
#define BITMAP_BPP(Bp_Bitmap)    ((*((BYTE far *)(Bp_Bitmap))) & 0x07)
#define BITMAP_WIDTH(Bp_Bitmap)     (*((WORD far *)(((BYTE far *)(Bp_Bitmap)) + BITMAP_WIDTH_OFFSET)))
#define BITMAP_HEIGHT(Bp_Bitmap) (*((WORD far *)(((BYTE far *)(Bp_Bitmap)) + BITMAP_HEIGHT_OFFSET)))

#define OSD_ROTATE_TEXT       BIT4
#define HGLT_WND_EN_CMD_BIT      BIT6  // highlight window enable bit

//---enums---
#pragma option -b.

// TileSet IDs
enum
{
   FIRST_TILESET    = 0,
   SECOND_TILESET    = 1
};

// Tile control field values
enum
{
   OSD_TILE_DISABLED = 0, // Tile Disabled
   OSD_COLOR2        = 1, // 1 bpp (2 colors)
   OSD_COLOR4        = 2, // 2 bpp (4 colors)
   OSD_COLOR16       = 3, // 4 bpp (16 colors)
   OSD_COLOR256      = 4, // 8 bpp (256 colors)
   OSD_COLOR1        = 5, // 0 bpp (1 color)
   OSD_TTX_MODE      = 6, // TTX (16 bits) mode
   OSD_ENH_TTX_MODE  = 7  // TTX (16 bits) mode, THIS MODE IS SUPPORTED ONLY IN FLI30436
};

// HighLight commands
enum
{
   HIGHLIGHT_CMD_DISABLE    = 0,
   HIGHLIGHT_CMD_ENABLE    = 1
};

//tile attributes bit masks
enum
{
   TILE_ATTR_COLOR_BITS       = 0x07,
   TILE_ATTR_TRANS_BIT           = BIT3,
   TILE_ATTR_BLEND_BIT           = BIT4,
   TILE_ATTR_BLINK_BIT           = BIT5,
   TILE_ATTR_TRANS_BEHAVIOR_BIT  = BIT6,
   TILE_ATTR_BLINK_BEHAVIOR_BIT  = BIT7
};

//enumeration for the highlight windows attributes bits
enum
{
   HIGHLIGHT_ATTR_COLOR_INDEX_BITS     = 0x0F,
   HIGHLIGHT_ATTR_TRANS_BIT         = BIT4,
   HIGHLIGHT_ATTR_TRANS_BEHAVIOR_BIT   = BIT5,
   HIGHLIGHT_ATTR_BLINK_BIT         = BIT6
};

// Enable/Disable commands
enum
{
   TILE_FEATURE_DISABLE = 0,
   TILE_FEATURE_ENABLE     = 1
};

#pragma option -b-

//---Macros---
#define TEST_BIT(Value, BitMask) (((Value) & (BitMask)) ? 1 : 0)
#define GET_TILE_ATTR_COLOR(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_COLOR_BITS)
#define GET_TILE_ATTR_TRANS(Stp_OsdTile) \
                     (TEST_BIT(((Stp_OsdTile)->B_Attr), TILE_ATTR_TRANS_BIT))
#define GET_TILE_ATTR_BLEND(Stp_OsdTile) \
                     (TEST_BIT(((Stp_OsdTile)->B_Attr), TILE_ATTR_BLEND_BIT))
#define GET_TILE_ATTR_BLINK(Stp_OsdTile) \
                     (TEST_BIT(((Stp_OsdTile)->B_Attr), TILE_ATTR_BLINK_BIT))
#define GET_TILE_ATTR_TRANS_BEHAVIOR(Stp_OsdTile) \
                     (TEST_BIT(((Stp_OsdTile)->B_Attr), TILE_ATTR_TRANS_BEHAVIOR_BIT))
#define GET_TILE_ATTR_BLINK_BEHAVIOR(Stp_OsdTile) \
                     (TEST_BIT(((Stp_OsdTile)->B_Attr), TILE_ATTR_BLINK_BEHAVIOR_BIT))

#define SET_TILE_ATTR_COLOR(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= ((B_Value) & 0x07))
#define SET_TILE_ATTR_TRANS(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= (((B_Value) & 0x01) << 0x03))
#define SET_TILE_ATTR_BLEND(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= (((B_Value) & 0x01) << 0x04))
#define SET_TILE_ATTR_BLINK(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= (((B_Value) & 0x01) << 0x05))
#define SET_TILE_ATTR_TRANS_BEHAVIOR(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= (((B_Value) & 0x01) << 0x06))
#define SET_TILE_ATTR_BLINK_BEHAVIOR(Stp_OsdTile, B_Value) \
                     (((Stp_OsdTile)->B_Attr) |= (((B_Value) & 0x01) << 0x07))

#define TEST_TILE_ATTR_TRANS(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_TRANS_BIT)
#define TEST_TILE_ATTR_BLEND(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_BLEND_BIT)
#define TEST_TILE_ATTR_BLINK(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_BLINK_BIT)
#define TEST_TILE_ATTR_TRANS_BEHAVIOR(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_TRANS_BEHAVIOR_BIT)
#define TEST_TILE_ATTR_BLINK_BEHAVIOR(Stp_OsdTile) \
                     (((Stp_OsdTile)->B_Attr) & TILE_ATTR_BLINK_BEHAVIOR_BIT)

#define HIGHLIGHT_ATTR_COLOR_INDEX(Stp_OsdTileHlght) \
                           (((Stp_OsdTileHlght)->B_Attr) & HIGHLIGHT_ATTR_COLOR_INDEX_BITS)
#define HIGHLIGHT_ATTR_TRANS(Stp_OsdTileHlght) \
                           (TEST_BIT(((Stp_OsdTileHlght)->B_Attr), HIGHLIGHT_ATTR_TRANS_BIT))
#define HIGHLIGHT_ATTR_TRANS_BEHAVIOR(Stp_OsdTileHlght) \
                           (TEST_BIT(((Stp_OsdTileHlght)->B_Attr), HIGHLIGHT_ATTR_TRANS_BEHAVIOR_BIT))
#define HIGHLIGHT_ATTR_BLINK(Stp_OsdTileHlght) \
                           (TEST_BIT(((Stp_OsdTileHlght)->B_Attr), HIGHLIGHT_ATTR_BLINK_BIT))

// structure of constants for different bpp sizes
typedef struct
{
   WORD W_OsdDivByBpp;
   WORD W_OsdBppMask;
   WORD W_OsdBppShift;
   WORD W_OsdUpdateWidth;
   WORD W_OsdColorIndexMask;
} ST_BPP_CONST;

// Defines an RGB color value in OSD CLUT in SRAM
#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE  B_Red;      // red component
   BYTE  reserved;   // reserved
   BYTE  B_Blue;     // blue component
   BYTE  B_Green; // green component
} ST_OSD_RGB;
typedef ST_OSD_RGB far *STP_OSD_RGB;

// Defines a tile data structures which is used as a parameter
// in tile management functions
typedef struct
{
   WORD  W_Xpos;     //x start position in pixels startinh from 0
   //it has to be an even number
   WORD  W_Ypos;     //y start position in pixels starting from 0
   WORD  W_Width; //width in pixels
   //it has to be an even number
   WORD  W_Height;   //height in pixels

   // The B_Attr attributes field has the following structure:
   // Bits:
   // 2:0      Color Depth
   //    OSD_COLOR1 - 0 bpp (1 color)
   //    OSD_COLOR2 - 1 bpp (2 colors)
   //    OSD_COLOR4 - 2 bpp (4 colors)
   //    OSD_COLOR16 - 4 bpp (16 colors)
   //    OSD_COLOR256 - 8 bpp (256 colors)
   // 3     Transparency
   // 4     Blending
   // 5     Blinking
   // 6     Trans_Behavior
   // 7     Blink_Behavior
   BYTE  B_Attr;

   WORD  W_Color; //colour attributes for a tile
   //which is a 12-bit index referencing
   //to a part OSD CLUT for this tile

   DWORD D_BmpAddr;  //SDRAM start address in bursts (64 bytes)
   //for a memory dedicated for this tile
} ST_OSD_TILE;
typedef ST_OSD_TILE far *STP_OSD_TILE;

// Defines the highlight window data structure which is used as a parameter
// in highlight windows management functions
typedef struct
{
   WORD W_Xpos;      //x start position in pixels relative to a tile x start
   //it has to be an even number
   WORD W_Ypos;      //y start position in pixels relative to a tile y start
   WORD W_Width;     //width in pixels
   //it has to be an even number
   WORD W_Height;    //height in pixels

   // The B_Attr attributes field defines the highlight windows properties and has the following meaning:
   // Bits     Description
   // 3:0      The upper 4 bits of the base color index of the highlight area
   // 4     Transparency - when set to 1, pixels with color index 0 become transparent
   // 5     Trans_Behavior - when Trans = 1 and Trans_Behavior = 0, all BACKGROUND pixels
   //       that are inside the enabled HILITE WINDOW are transparent.
   //       If the tile is overlaying the graphics/video stream, the graphics/video
   //       stream is visible in the transparent pixel region.
   //       If the tile is overlaying another tile, the pixels produced by the
   //       overlaid tile are visible in the transparent region.
   //       If Trans = 1 and rans_Behavior = 1, all BACKGROUND pixels that are inside
   //       the enabled HILITE WINDOW are transparent to underlying graphics/video
   //       stream regardless of what is below the tile.
   // 6  Blink - when BLINK = 1 and hi-light window is enabled, all pixels in the tile
   //       that are inside hi-light window are forced to BACKGROUND pixels during
   //       the blink "off" interval when the host BLINK_EN bit is set.
   //          If BLINK_BEHAVIOR = '0', then foreground pixels within hi-light window revert
   //       to background color of hi-light window.  If BLINK_BEHAVIOR = '1', then
   //       foreground pixels within hi-light window 4 revert to background color of the tile.
   // 7  Reserved
   BYTE B_Attr;
} ST_OSD_TILE_HIGHLIGHT;
typedef ST_OSD_TILE_HIGHLIGHT far *STP_OSD_TILE_HIGHLIGHT;

// Defines a bitmap image header which precede actual compressed pixel data
typedef struct
{
   // The B_Attr attributes field bitmap properties which has the folowing meaning
   // Bits     Description
   // 2:0      Bit-per-pixel rate
   // 7:3      Reserved
   BYTE B_Attr;

   WORD W_Width;     //width in pixels
   WORD W_Height;    //height in pixels
} ST_OSD_BITMAP_HDR;
#pragma pack(pop)

// Definition of a tile record in COMMAND SRAM
// word #0 in the spec is at the highest address,
// word #19 is at the lowest address
#pragma warn -bbf
typedef struct
{
   // highlight windows area in a tile record
   struct
   {
      // OSD Command HighLite Control Word #2
      WORD W_HStartHiLite     :10;  //x start relative to a tile start position
      WORD W_HEndHiLite0_5 : 6;  //x end relative to a tile start position [5:0]

      // OSD Command HighLite Control Word #1
      WORD W_VEndHiLite    :11;  //y end relative to a tile start position
      WORD W_HEndHiLite6_9 : 4;
      WORD W_HiLiteTrans      : 1;

      // OSD Command HighLite Control Word #0
      WORD W_VStartHiLite     :11;  // line # relative to tile where 
      WORD W_HiLiteIndex      : 4;  // bpp control of HiLite window
      WORD W_HiLiteBlink      : 1;
   } HighLight[MAX_NUMBER_OF_HILITE_WINDS];

   // OSD Command Word #7
   WORD W_ColorIndex       :12;  //specifies color attributes for a tile
   WORD W_HL4TransBehavior    : 1;  //specifies a transparency behaviour for a highlight window #4
   WORD W_HL3TransBehavior    : 1;  //specifies a transparency behaviour for a highlight window #3
   WORD W_HL2TransBehavior    : 1;  //specifies a transparency behaviour for a highlight window #2
   WORD W_HL1TransBehavior    : 1;  //specifies a transparency behaviour for a highlight window #1

   // OSD Command Word #6
   //only bits [9:0] will be taken into account by hw
   //W_HStart has to be multiplied by 2 to get a real start position in pixels
   //it means a tile could start only on even pixels for X
   WORD W_HStart;

   // OSD Command Word #5
   //only bits [9:0] will be taken into account by hw
   //W_HWidth has to be multiplied by 2 to get a real width in pixels
   //it means a tile width could be only an even number of pixels
   WORD W_HWidth;

   // OSD Command Word #4
   //only bits [9:0] will be taken into account by hw
   //W_HStride has to be multiplied by 2 to get a real stride in pixels
   //hw internally will adjust this value to the nearest byte boundaries value
   //this field is used to simplify RMW during of bitmap drawing
   //or to decrease a bandwidth for a tile per one scan line
   WORD W_HStride;

   // OSD Command Word #3
   WORD W_SdramAddrLo;              // lower 16 bits of SDRAM address

   // OSD Command Word #2
   //only bits [10:0] will be taken into account by hw
   WORD W_VHeight;

   // OSD Command Word #1
   //only bits [10:0] will be taken into account by hw
   WORD W_VStart;

   // OSD Command Word #0
   //we split it into two bytes to improve performance during of SDRAM address calculation
   //which happens very often
   BYTE W_TileControl         : 3;  // Color depth of a tile
   BYTE W_Transparent         : 1;
   BYTE W_Blend            : 1;
   BYTE W_Blink            : 1;
   BYTE W_TransBehavior    : 1;
   BYTE W_BlinkBehavior    : 1;

   //only bits [3:0] will be taken into account by hw
   BYTE W_SdramAddrHi;              // SDRAM address bits [19:16]
} ST_SRAM_TILE;
typedef ST_SRAM_TILE far *STP_SRAM_TILE;
#pragma warn +bbf

enum
{
   OSD_ENH_TTX_ROW_0    = 0,
   OSD_ENH_TTX_ROW_1    = 1,
   OSD_ENH_TTX_ROW_2    = 2,
   OSD_ENH_TTX_ROW_3    = 3,
   OSD_ENH_TTX_ROW_4    = 4,
   OSD_ENH_TTX_ROW_5    = 5,
   OSD_ENH_TTX_ROW_6    = 6,
   OSD_ENH_TTX_ROW_7    = 7,
   OSD_ENH_TTX_ROW_8    = 8,
   OSD_ENH_TTX_ROW_9    = 9,
   OSD_ENH_TTX_ROW_10      = 10,
   OSD_ENH_TTX_ROW_11      = 11,
   OSD_ENH_TTX_ROW_12      = 12,
   OSD_ENH_TTX_ROW_13      = 13,
   OSD_ENH_TTX_ROW_14      = 14,
   OSD_ENH_TTX_ROW_15      = 15,
   OSD_ENH_TTX_ROW_16      = 16,
   OSD_ENH_TTX_ROW_17      = 17,
   OSD_ENH_TTX_ROW_18      = 18,
   OSD_ENH_TTX_ROW_19      = 19,
   OSD_ENH_TTX_ROW_20      = 20,
   OSD_ENH_TTX_ROW_21      = 21,
   OSD_ENH_TTX_ROW_22      = 22,
   OSD_ENH_TTX_ROW_23      = 23,
   OSD_ENH_TTX_ROW_24      = 24,
   OSD_ENH_TTX_ROW_25      = 25,
   OSD_ENH_TTX_ALL_ROWS = 26,
   OSD_ENH_TTX_TOP         = 27,
   OSD_ENH_TTX_BOTTOM      = 28,
   OSD_ENH_TTX_SCREEN      = 29,
   OSD_ENH_TTX_ALL_AREA = 30
};

enum
{
   OSD_ENH_TTX_ENABLE_CHAR_GRID  = 0,
   OSD_ENH_TTX_TRANSPARENCY      = 1,
   OSD_ENH_TTX_BLENDING       = 2
};

// Definition of a tile record in COMMAND SRAM in the Enhanced Teletext mode
// word #0 in the spec is at the highest address,
// word #19 is at the lowest address
#pragma warn -bbf
typedef struct
{
   // OSD Command Word #19
   WORD W_ColorIndexRow24  : 5;  // color index of row 24
   WORD W_ColorIndexRow25  : 5;  // color index of row 25
   WORD reserved19         : 6;

   // OSD Command Word #18
   WORD W_ColorIndexRow21  : 5;  // color index of row 21
   WORD W_ColorIndexRow22  : 5;  // color index of row 22
   WORD W_ColorIndexRow23  : 5;  // color index of row 23
   WORD reserved18         : 1;

   // OSD Command Word #17
   WORD W_ColorIndexRow18  : 5;  // color index of row 18
   WORD W_ColorIndexRow19  : 5;  // color index of row 19
   WORD W_ColorIndexRow20  : 5;  // color index of row 20
   WORD reserved17         : 1;

   // OSD Command Word #16
   WORD W_ColorIndexRow15  : 5;  // color index of row 15
   WORD W_ColorIndexRow16  : 5;  // color index of row 16
   WORD W_ColorIndexRow17  : 5;  // color index of row 17
   WORD reserved16         : 1;

   // OSD Command Word #15
   WORD W_ColorIndexRow12  : 5;  // color index of row 12
   WORD W_ColorIndexRow13  : 5;  // color index of row 13
   WORD W_ColorIndexRow14  : 5;  // color index of row 14
   WORD reserved15         : 1;

   // OSD Command Word #14
   WORD W_ColorIndexRow9   : 5;  // color index of row 9
   WORD W_ColorIndexRow10  : 5;  // color index of row 10
   WORD W_ColorIndexRow11  : 5;  // color index of row 11
   WORD reserved14         : 1;

   // OSD Command Word #13
   WORD W_ColorIndexRow6   : 5;  // color index of row 6
   WORD W_ColorIndexRow7   : 5;  // color index of row 7
   WORD W_ColorIndexRow8   : 5;  // color index of row 8
   WORD reserved13         : 1;

   // OSD Command Word #12
   WORD W_ColorIndexRow3   : 5;  // color index of row 3
   WORD W_ColorIndexRow4   : 5;  // color index of row 4
   WORD W_ColorIndexRow5   : 5;  // color index of row 5
   WORD reserved12         : 1;

   // OSD Command Word #11
   WORD W_ColorIndexRow0   : 5;  // color index of row 0
   WORD W_ColorIndexRow1   : 5;  // color index of row 1
   WORD W_ColorIndexRow2   : 5;  // color index of row 2
   WORD reserved11         : 1;

   // OSD Command Word #10
   WORD W_HWidth        : 10; // tile width in pixels divided by 2
   WORD reserved10         : 6;

   // OSD Command Word #9
   WORD W_VHeight       : 11; // tile height in lines
   WORD reserved9       : 2;
   WORD W_Transparency     : 1;  // enables/disables tile transparency except characters grid area
   WORD W_Blending         : 1;  // enables/disables tile blending
   WORD W_CharHeight_hi : 1;  // character height in lines bit[6]

   // OSD Command Word #8
   WORD W_Ypos          : 11; // tile Y start position in lines
   WORD W_ColorIndexBottom : 5;  // color index of bottom of the screen

   // OSD Command Word #7
   WORD W_Xpos          : 10; // tile X start position in pixels divided by 2
   WORD W_CharHeight_lo : 6;  // character height in lines bits[5:0]

   // OSD Command Word #6
   WORD W_CharRegionXpos   : 10; // X start position of the character grid area in pixels divided by 2
   WORD reserved6       : 6;

   // OSD Command Word #5
   WORD W_Columns       : 8;  // number of columns in the character grid area minus one
   WORD W_CharWidth     : 6;  // character width in pixels divided by 2 minus one
   WORD reserved5       : 2;

   // OSD Command Word #4
   WORD W_Stride        : 10; // tile stride in 16-bit words ( (2 + W_CharWidth[5:3] + 1) * (W_Columns + 1) )
   WORD reserved4       : 6;

   // OSD Command Word #3
   WORD W_SdramAddrLo;           // SDRAM address bits [15:0]

   // OSD Command Word #2
   WORD W_CharRegionHeight : 11; // character grid area height in lines (# of rows * W_CharHeight)
   WORD reserved2       : 5;

   // OSD Command Word #1
   WORD W_CharRegionYpos   : 11; // Y start position of the character grid area in lines
   WORD reserved1       : 5;

   // OSD Command Word #0
   WORD W_TileControl      : 3;  // color depth of a tile
   WORD W_ColorIndexTop : 5;  // color index of top of the screen
   WORD W_SdramAddrHi      : 4;  // SDRAM address bits [19:16]
   WORD W_EnableCharRegion : 1;  // enables/disables characters grid area
   WORD reserved0       : 3;
} ST_SRAM_ENH_TTX_TILE;
typedef ST_SRAM_ENH_TTX_TILE far *STP_SRAM_ENH_TTX_TILE;
#pragma warn +bbf

typedef struct
{
   BYTE B_TileId;    //tile id of the tile where to draw a character
   WORD W_Xpos;      //x pos within the tile in pixels
   WORD W_Ypos;      //y pos within the tile in lines
   WORD W_CharWidth;   //character width in pixels
   WORD W_CharHeight;   //character height in lines
   WORD W_CharSize;  //character size in bytes
   WORD W_BgColor;     //background color index of the character from 0 to 255
   WORD W_FgColor;      //foreground color index of the character from 0 to 255
} gmt_OsdUniCharacter;

typedef struct
{
   gmt_OsdUniCharacter St_Char;           //character description
   WORD W_NumberOfFonts;               //number of fonts
   BYTE B_InterCharSpace;              //inter glyph spacing in pixels
   BYTE const far * const far *Wp_Fonts; //pointer to an array containing pointers to font tables
   WORD const far * const far *Wp_Maps;  //pointer to an array containing pointers to map files
   WORD const far *Wp_Control;           //pointer to an array containing control information for each font
   //<number of characters in the font>, <the lowest Unicode index>, <the highest Unicode index>
} gmt_OsdUniString;

//---functions----
void far gm_OsdShow(void);//jmp_tbl
void far gm_OsdHide(void);//jmp_tbl
gmt_RET_STAT huge gm_OsdConfigureTilesEx(WORD W_NumberOfTiles, WORD W_Tileset0Address, WORD W_Tileset1Address);//jmp_tbl
gmt_RET_STAT huge gm_OsdConfigureTiles(WORD W_NumberOfTiles, WORD W_Tileset0Address, WORD W_Tileset1Address);//jmp_tbl
void huge gm_OsdReset(void);//jmp_tbl
void huge gm_OsdInitEx(WORD W_NumberOfTiles, WORD W_Tileset0Address, WORD W_Tileset1Address);//jmp_tbl
void huge gm_OsdInit(WORD W_NumberOfTiles, WORD W_Tileset0Address, WORD W_Tileset1Address); //jmp_tbl
WORD huge gm_OsdGetNumberOfTiles(void);//jmp_tbl
WORD huge gm_GetSelectedTileset(void);//jmp_tbl
void huge gm_SetSelectedTileset(WORD W_SelTileset);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetTile(WORD W_TileId, ST_OSD_TILE const far *Stp_OsdTile, WORD W_Flags);//jmp_tbl
gmt_RET_STAT huge gm_OsdGetTile(WORD W_TileId, ST_OSD_TILE *Stp_OsdTile);//jmp_tbl
gmt_RET_STAT huge gm_OsdEnableTile(WORD W_TileId);//jmp_tbl
gmt_RET_STAT huge gm_OsdDisableTile(WORD W_TileId);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileTransparency(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileBlink(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileBlend(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileTransBehavior(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileBlinkBehavior(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetTilePosition(WORD W_TileId, WORD W_Xpos, WORD W_Ypos);//jmp_tbl
gmt_RET_STAT huge gm_OsdGetTilePosition(WORD W_TileId, WORD *Wp_Xpos, WORD *Wp_Ypos);//jmp_tbl
gmt_RET_STAT huge gm_OsdGetTileSize(WORD W_TileId, WORD *Wp_Xsize, WORD *Wp_Ysize);//jmp_tbl
void far gm_OsdSwitchTileSet(void);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileHighlight(WORD W_TileId, WORD W_Command,  ST_OSD_TILE_HIGHLIGHT const far *Stp_OsdTileHighLight);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileHighLightColourIndex(WORD W_TileId, WORD W_Flags);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileHighLightTransparency(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileHighLightBlink(WORD W_TileId, WORD W_Command);//jmp_tbl
gmt_RET_STAT huge gm_OsdTileHighLightTransBehavior(WORD W_TileId, WORD W_Command);//jmp_tbl
WORD huge gm_OsdLoadClutByOcmPollingEx(WORD W_StartIndex, WORD W_Size, WORD const far *Wp_ColorData);//jmp_tbl
gmt_RET_STAT huge gm_OsdLoadClutByOcmPolling(WORD W_StartIndex, WORD W_Size, WORD const far *Wp_ColorData); //jmp_tbl
WORD huge gm_OsdLoadClutByDmaEx(WORD W_StartIndex, WORD W_Size, WORD const far *Wp_ColorData);//jmp_tbl
gmt_RET_STAT huge gm_OsdLoadClutByDma(WORD W_StartIndex, WORD W_Size, WORD const far *Wp_ColorData);//jmp_tbl
gmt_RET_STAT huge gm_OsdFillRectangle(WORD W_TileId, WORD W_Xpos, WORD W_Ypos, WORD W_Width, WORD W_Height, WORD W_ColorIndex);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetFontVector(BYTE const far * const far * const far Bp_FontVector, WORD W_Count);//jmp_tbl
gmt_RET_STAT huge gm_OsdPrintTextEx(WORD W_TileId, WORD W_FontId, WORD W_Xpos, WORD W_Ypos, WORD W_GlyphsNumber, BYTE const far* Bp_Text,    BYTE const far* Bp_ColorMap, WORD W_Flags);//jmp_tbl
gmt_RET_STAT huge gm_OsdDrawUncomprImage(WORD W_TileId, WORD W_Xpos, WORD W_Ypos, BYTE const far* Bp_Bitmap, BYTE const far* Bp_ColorMap, WORD W_Flag);//jmp_tbl
void huge gm_OsdSetGlobalFontId(WORD W_FontId);//jmp_tbl
BYTE const far * huge gm_OsdGetGlyphHeader(WORD W_GlyphId);//jmp_tbl
void far gm_OsdRotate1bppPixelDataEx(WORD W_Width, WORD W_Height,   BYTE far *Bp_Src, BYTE far *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate2bppPixelDataEx(WORD W_Width, WORD W_Height,   BYTE far *Bp_Src, BYTE far *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate4bppPixelDataEx(WORD W_Width, WORD W_Height,   BYTE far *Bp_Src, BYTE far *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate8bppPixelDataEx(WORD W_Width, WORD W_Height,   BYTE far *Bp_Src, BYTE far *Bp_Dest); //jmp_tbl
void far gm_OsdRotate1bppPixelData(WORD W_Width, WORD W_Height, BYTE near *Bp_Src, BYTE near *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate2bppPixelData(WORD W_Width, WORD W_Height, BYTE near *Bp_Src, BYTE near *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate4bppPixelData(WORD W_Width, WORD W_Height, BYTE near *Bp_Src, BYTE near *Bp_Dest, WORD W_HeightInBytes);//jmp_tbl
void far gm_OsdRotate8bppPixelData(WORD W_Width, WORD W_Height, BYTE near *Bp_Src, BYTE near *Bp_Dest);//jmp_tbl

void huge gm_RenderBufferInit(WORD W_BufferSize, BYTE fdata* Bp_Buffer);//jmp_tbl
void huge gm_DecompressInit(WORD W_BufferSize, BYTE near* Bp_Buffer);//jmp_tbl
gmt_RET_STAT huge gm_OsdPixelExpansionBufferInit(BYTE ndata * Bp_PixelExpansionTable, WORD W_PixelExpansionTableSize);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetEnhTtxTile(WORD W_TileId, WORD W_Xpos, WORD W_Ypos, WORD W_Width, WORD W_Height, WORD W_LeftOffset, WORD W_TopOffset, BYTE B_Columns, BYTE B_Rows, BYTE B_CharWidth, BYTE B_CharHeight);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetEnhTtxTileColor(WORD W_TileId, BYTE B_Row, BYTE B_Color);//jmp_tbl
gmt_RET_STAT huge gm_OsdSetEnhTtxTileAttr(WORD W_TileId, BYTE B_Attr, BYTE B_Value);//jmp_tbl
gmt_RET_STAT huge gm_OsdDrawUnicodeCharacter(gmt_OsdUniCharacter const far *Sp_CharDescr, BYTE const far *Bp_CharFontData);//jmp_tbl

//*************************************************************************************************
// AFP.C
//*************************************************************************************************
void huge gm_AfpDetectInit(void);//jmp_tbl
void huge gm_AfpSetVmPalFlag(BYTE B_PalFlag);//jmp_tbl
void huge gm_AfpSet22GetNoiseSThrFunc(DWORD far (*pNewFunc)(void));//jmp_tbl

#pragma option -b.

//******************************************************************************
// ir.c
//******************************************************************************
enum IrProtocols
{
   IR_PROTOCOL_RECS80   = 0,
   IR_PROTOCOL_RC5,
};

enum IrProtocolFlags
{
   IR_FLAG_NONE         = 0,  // None
   IR_FLAG_REPEAT       = BIT1, // Repeat Key Packet
   IR_FLAG_MULT_START      = BIT2, // Packet contains multiple start pulses
   IR_FLAG_POWER        = BIT3, // Power Key Packet
   IR_FLAG_REVERSE         = BIT4, // Reverse Bit order MSB...LSB
};

// Packet Definition
typedef struct
{
   BYTE        B_Flags;
   BYTE        B_Start0Min;
   BYTE        B_Start0Max;
   BYTE        B_Start1Min;
   BYTE        B_Start1Max;
   BYTE        B_TroughMin;      // RECS80         RC5
   BYTE        B_TroughMax;      // ------         ---------
   BYTE        B_Data0Min;       // Logic0Min      SingleMin
   BYTE        B_Data0Max;       // Logic0Max      SingleMax
   BYTE        B_Data1Min;       // Logic1Min      DoubleMin
   BYTE        B_Data1Max;       // Logic1Max      DoubleMax
   BYTE        B_PayloadCount;
   DWORD       D_AddrMask;
   BYTE        B_AddrShift;
   DWORD       D_DataMask;
   BYTE        B_DataShift;
   WORD        W_CmdID;
} gmt_IrPacket;

//internal data structure
#define MINIMAL_ARRAY_SIZE 1
#pragma pack(push)
#pragma pack(1)
typedef struct
{
   BYTE        B_Protocol;
   WORD        W_Config;
   WORD        W_Timing;
   BYTE        B_Period;
   BYTE        B_NumPackets;
   gmt_IrPacket   B_Packets[MINIMAL_ARRAY_SIZE];
} gmt_IrProtocol;
#pragma pack(pop)

//******************************************************************************
//  OTP.c
//******************************************************************************
extern BYTE B_OTP_Init;
typedef struct
{
   DWORD DW_LpmTclkClock;	// Hz
   DWORD DW_LpmCpuClock;	// Hz
   DWORD DW_MissionCpuClock;// Hz
   DWORD DW_RemovalTime; // Nanosecond
   DWORD DW_RecoveryTime; // Nanosecond
   DWORD DW_AutoOffTime; //Microsecond
} OTP_INIT_PARAMS;
extern OTP_INIT_PARAMS S_OTP_Param;

gmt_RET_STAT huge gm_OTPInitialize(const OTP_INIT_PARAMS far *param);//jmp_tbl
gmt_RET_STAT huge gm_OTPReadMem(DWORD DW_Address, BYTE far* Bp_Data, WORD W_SizeInBytes);//jmp_tbl
gmt_RET_STAT huge gm_OTPWriteMem(DWORD DW_Address, const BYTE far* Bp_Data, WORD W_SizeInBytes);//jmp_tbl

//******************************************************************************
//  AuxRx.c / AuxRxI2C.c
//******************************************************************************
#define CONVERT_AUXCH_TO_AUXHANDLER(Ch) (Ch+AUX_C0_USE)

#define MAX_DDC2BI_MSG_SIZE           132
#define MAX_I2C_SW_ADDRESSES          16

#define EXEC_MEM_DEBUG                106
#define NULL_MSG_LENGTH         	  3

#define AUXTX_I2C_SW_MARKER   0x0080
#define AUXTX_I2C_ADDR_MASK   0x007f
#define AUXTX_I2C_HW_ADDR(A)   (AUXTX_I2C_ADDR_MASK & (WORD)(A))
#define AUXTX_I2C_SW_ADDR(A)   ((AUXTX_I2C_ADDR_MASK & (WORD)(A)) | AUXTX_I2C_SW_MARKER)
#define AUX_I2C_IS_SW_TR(A)   ((A) & AUXTX_I2C_SW_MARKER)

#define DDC2B_SRC_ADDRESS               0x51
#define DDC2B_DEST_ADDRESS              0x6E

typedef BOOL (far* T_AuxRxI2C_ExtCallback) (BYTE B_Addr, BYTE* PB_Buff, BYTE* PB_Length, BYTE B_Stop, BYTE* PB_Status, AUX_CH B_AuxCh);
typedef void far (* DPRXAUX_LL_REQUEST_TYPE)(BYTE B_Request, DWORD DW_Addr, BYTE  B_Len, AUX_CH B_AuxCh);
typedef BOOL (far* gmt_DPRX_AUX_EXT_HANDLER) (DWORD W_Addr, BYTE B_Command, BYTE B_Length, AUX_CH B_AuxCh);
typedef BOOL (far* gmt_DDC2BI_EXT_FUNC)(BYTE B_DevID, BYTE B_Len, BYTE *Bp_Buff, AUX_CH B_AuxCh);


//Module internal data
typedef struct
{
   gmt_DPRX_AUX_EXT_HANDLER ExtSWI2CHandler;
   gmt_DPRX_AUX_EXT_HANDLER ExtI2CHandler;

}gmt_DPRX_AUX_I2C_INTERNAL_DATA;

typedef struct SW_TransactionStruct   // SW Transactions Struct
{
   BYTE  AB_Data[MAX_DDC2BI_MSG_SIZE];
   WORD  W_CurCount;
   WORD  W_SendDevID;
   BYTE  B_Dir;
   BYTE  B_DevID;
   BYTE  B_SWDevIdCount;
   BYTE  B_SWDevId[MAX_I2C_SW_ADDRESSES];
   BYTE  B_ReplyReady;
   BYTE  B_NullMsgPtr;
}SW_TransactionStruct_t;

extern DPRXAUX_LL_REQUEST_TYPE Fp_DpRxAuxLLNativeRequest;
extern SW_TransactionStruct_t SW_Transaction_ModuleData[AUX_CH_END];
extern SW_TransactionStruct_t *pSW_Transaction_ModuleDataGlobal[AUX_CH_END];
extern gmt_DPRX_AUX_I2C_INTERNAL_DATA AuxRxI2CModuleData;
extern BYTE ExecuteMemTest[AUX_CH_END];

//extern TAuxRxI2C_InternalData* P_AuxRxI2CInternalData;
//extern SW_TransactionStruct_t* pSW_Transaction_ModuleData;
extern gmt_DPRX_AUX_I2C_INTERNAL_DATA* P_AuxRxI2CModuleData;
//extern BYTE* Bp_CurMsg;
extern gmt_DDC2BI_EXT_FUNC Fp_ExtDDC2iCheckCS;

void huge gm_DpRxAuxRequestISR(void);//jmp_tbl
void huge gm_SetDpRxAuxLLNativeRequest(DPRXAUX_LL_REQUEST_TYPE Fp_NewFunc);//jmp_tbl
BOOL huge gm_DpRxAuxGetChunkData(BYTE* PB_Dest, BYTE B_Len, AUX_CH B_AuxCh);//jmp_tbl
void huge gm_DpRxAuxSetReplyData(BYTE* PB_Mem, BYTE B_Len, AUX_CH B_AuxCh);//jmp_tbl
void huge gm_DpRxAuxSetReplyCmd(BYTE B_RepCmd, AUX_CH B_AuxCh);//jmp_tbl
void huge gm_DpRxAuxPhyInit(void);//jmp_tbl
gmt_RET_STAT huge gm_DpRxAuxInit(void);//jmp_tbl
WORD huge gm_AuxRxI2C_SW_TransInit(BYTE* PB_StartRAM, AUX_CH B_AuxCh);//jmp_tbl
BOOL huge gm_AuxRxI2C_SW_IsSWTransaction(DWORD DW_Addr, AUX_CH B_AuxCh);//jmp_tbl
void huge gm_AuxRxI2C_SW_AddSWDevId(BYTE B_I2CDevId, AUX_CH B_AuxCh);//jmp_tbl
void huge gm_AuxRxI2C_SW_ClearSWDevIdTable(AUX_CH B_AuxCh);//jmp_tbl
BYTE huge gm_AuxRxI2C_SW_Handler(WORD W_Address, BOOL B_Dir, BYTE *Bp_Buffer, WORD W_Length, BYTE B_Stop, AUX_CH B_AuxCh);//jmp_tbl
BYTE* huge gm_AuxRxI2C_SW_GetBuff(AUX_CH B_AuxCh);//jmp_tbl
SW_TransactionStruct_t* huge gm_AuxRxI2C_SW_GetAddrModuleData(AUX_CH B_AuxCh);//jmp_tbl
BOOL huge gm_AuxRxI2C_SWCallback(DWORD W_Addr, BYTE B_Command, BYTE B_Length, AUX_CH B_AuxCh);//jmp_tbl
WORD huge gm_DpRxAuxLLI2CInit(BYTE* PB_StartRAM,gmt_DPRX_AUX_EXT_HANDLER ExtSWI2CFunc,gmt_DPRX_AUX_EXT_HANDLER ExtI2CFunc);//jmp_tbl
void huge gm_DpRxAuxLLI2CRequest(BYTE B_Request, DWORD DW_Addr, BYTE  B_Len, AUX_CH B_AuxCh);//jmp_tbl

//******************************************************************************
//  CBootUp.c
//******************************************************************************
#define FORCE_SA_CMD          0x3E
#define FORCE_SA_ACK          0x3F
#define FORCE_SA_NACK         0x4F

typedef enum
{
   FLASH_A = 0,
   FLASH_B = 1,
   FLASH_NUM = 2,
   FLASH_NONE = 0xff
}DUAL_FLASH_TYPE;

typedef enum
{
   FSA_NONE = 0,
   FSA_FROM_UART0,
   FSA_FROM_UART1,
   FSA_FROM_DDC2B_A,
   FSA_FROM_DDC2B_D,
   FSA_FROM_I2C_SLAVE,
   FSA_FROM_AUX,
}gmt_FSA_SRC;

extern BYTE FlashSelect;	//record the flash to jump into.
extern BYTE VerTag[FLASH_NUM];	//version tag
extern gmt_FSA_SRC gB_ForceSA_Src;

void huge gm_DualFlashSwitch(DUAL_FLASH_TYPE B_FlashSel);//jmp_tbl


//******************************************************************************
//  SpiFlash.c
//******************************************************************************
typedef struct FLASH_PARAMETERS
{
   BYTE           B_DevID;                  // Device ID
   BYTE           B_ManID;                  // Manufactur ID
   BYTE           B_TypeID;
}FLASH_DEVICE_PARAMETER_SET;

typedef enum FLASH_READID_TYPE
{
   FLASH_READID_TYPE1,
   FLASH_READID_TYPE2
}FLASH_READID_TYPE;

FLASH_DEVICE_PARAMETER_SET * huge gm_SpiGetParameters(void);//jmp_tbl
void huge gm_SpiDetectFlashDevice(FLASH_READID_TYPE B_ReadIdType, BYTE B_ReadIdCmd);//jmp_tbl
void huge gm_SpiStopTransaction(void);//jmp_tbl
void huge gm_SpiStartTransaction(void);//jmp_tbl
BYTE huge gm_SpiWriteByte(BYTE value);//jmp_tbl
BYTE huge gm_SpiReadByte(BYTE *pB_ReadData);//jmp_tbl

//******************************************************************************
//  Csm.c
//******************************************************************************
//Refer to Athena register spec v1.7 p205
typedef enum
{
   //LPM I2C MASTER USE
   MUX_DDC1_LpmI2cMaster,				//2:I2C master is configured for connection to external I2C slave device
   //GPROBE, ISP or TOOL USE
   MUX_DDC1_MisI2cSlv1,				//3:Allow ISP ON LPM DDC1 port
   MUX_DDC2_MisI2cSlv2,				//4:Allow ISP ON LPM DDC2 port
   MUX_DDC3_MisI2cSlv3,				//5:Allow ISP ON LPM DDC3 port
   MUX_DDC4_MisI2cSlv4,				//6:Allow ISP ON LPM DDC4 port
   //DPRX0 AUX USE
   MUX_DDC1_MisDpC0Aux2I2c_I2cSlv1,	//7:I2C slave1 is used for Mission DP1 AUX2I2C DDC
   MUX_DDC1_MisDpC0Aux2I2c_ExtDev,		//8:External device on DDC1 por used for Mission: DP1 AUX2I2C DDC
   MUX_DDC1_LpmDpC0Aux2I2c_I2cSlv1,	//9:I2C slave1 is used for LPM DP1 AUX2I2C DDC
   MUX_DDC1_LpmDpC0Aux2I2c_ExtDev,		//10:External device on DDC1 port used for LPM: DP1 AUX2I2C DDC
   //DPRX1 AUX USE
   MUX_DDC3_MisDpC2Aux2I2c_I2cSlv3,	//11:I2C slave3 is used for Mission DP2 AUX2I2C DDC
   MUX_DDC3_MisDpC2Aux2I2c_ExtDev,		//12:External device on DDC3 port used for Mission DP2 AUX2I2C master
   MUX_DDC3_LpmDpC2Aux2I2c_I2cSlv3,	//13:I2C slave3 is used for DP2 AUX2I2C DDC
   MUX_DDC3_LpmDpC2Aux2I2c_ExtDev,		//14:External device on DDC3 port used for LPM: DP2 AUX2I2C DDC
   //HDMI USE
   MUX_DDC1_LpmI2cSlv1_MisHdmiCp,		//15:LPM_I2C_SLV1 & MIS_HDMI_HDCP_SLAVE on LPM_DDC1 PORT. EXT EDID OPTIONAL
   MUX_DDC2_LpmI2cSlv2_MisHdmiCp,		//16:LPM_I2C_SLV2 & MIS_HDMI_HDCP_SLAVE on LPM_DDC2 PORT. EXT EDID OPTIONAL
   MUX_DDC3_Lpmi2cSlv3_MisHdmiCp,		//17:LPM_I2C_SLV3 & MIS_HDMI_HDCP_SLAVE on LPM_DDC3 PORT. EXT EDID OPTIONAL
   //DVI USE
   MUX_DDC2_LpmI2cSlv2_MisDviCp,		//18:DVI is enabled. HDMI is not configured on i2C slave2 or DDC2 port. These are used by DVI
   //VGA USE
   MUX_DDC4_LpmI2cSlv4,				//29:LPM: I2C slave4 is used with external master on DDC4 port
   MUX_Num,
} I2cMuxSelect;

typedef enum
{
   PORT_SEL_DDC4 = 0,
   PORT_SEL_DDC4_DDC1 = 1,
   PORT_SEL_DDC4_DDC2 = 2,
   PORT_SEL_DDC4_DDC3 = 3,
} CSM_DDC_PORT_SEL;

typedef enum
{
   LPM_I2CSLV_PINS_TO_DDC2B = 0,
   LPM_I2CSLV_PINS_TO_SLAVE = 1,
} CSM_I2CSLV_PINS_ROUTE;

//Mapping to registers LPM_CSM_I2C_MUX_CTRL and MFP_SPARE1
typedef struct
{
   BYTE LpmI2cMSel;		//LPM_CSM_I2C_MUX_CTRL  bit[8]
   BYTE HdcpMapOverride;	//MFP_SPARE1  bit[1:0]
   BYTE LpmAuxSel;			//LPM_CSM_I2C_MUX_CTRL  bit[0]
   BYTE LpmDp1I2cMuxSel;	//LPM_CSM_I2C_MUX_CTRL  bit[5:4]
   BYTE LpmDp2I2cMuxSel;	//LPM_CSM_I2C_MUX_CTRL  bit[7:6]
   BYTE LpmHdmiI2cMuxSel;	//LPM_CSM_I2C_MUX_CTRL  bit[3:2]
   BYTE LpmDviI2cMuxSel;	//LPM_CSM_I2C_MUX_CTRL  bit[1]
} I2cMuxTableParams;

extern CSM_DDC_PORT_SEL BootstrapDdcPort;
extern CSM_I2CSLV_PINS_ROUTE CsmI2cSlvRoute;

void huge gm_CsmI2CMuxSelect (I2cMuxSelect MuxSel);//jmp_tbl
void huge gm_SetCsmI2CMuxPath(void); //jmp_tbl
void huge gm_SetDDCIRQMask(void); //jmp_tbl
void huge gm_GetBootstrapDDCPort (void);

//*************************************************************************************************
// DDC2BI.C
//*************************************************************************************************
BOOL huge gm_DDC2BiWriteCmdPkt(void far* pBuf);//jmp_tbl
PARSER_STATUS_t huge gm_DDC2Bi_Handler(void);//jmp_tbl
WORD huge gm_DDC2Bi_GetRcvCnt(void);//jmp_tbl
void huge gm_DDC2Bi_SetRcvCnt(WORD);//jmp_tbl
DDC2BI_RCV_STATES huge gm_DDC2Bi_GetRcvStatus(void);//jmp_tbl
void huge gm_DDC2Bi_Init(DDC2BI_CHANNEL channel, TDdc2bi fdata* pBuf);//jmp_tbl
void huge gm_DDC2Bi_Reinit(void);//jmp_tbl
void huge gm_DDC2Bi_SetDeviceID(BYTE B_NewDDC2BI_DEVICE_ID);//jmp_tbl
BOOL huge gm_DDC2BiWrite(const void far* pBuf, BYTE size);//jmp_tbl
TReadPkt huge gm_DDC2BiRead(BYTE far* pBuf);//jmp_tbl

//******************************************************************************
//  Main.c
//******************************************************************************
extern DWORD gDW_SystemOcmClk;

void huge gm_Delay1us(WORD W_Delay);//jmp_tbl

//******************************************************************************
//  IspPrimitives.c
//******************************************************************************
void huge gm_ISPRcvConfig(BYTE far*desBuff, WORD maxLen);//jmp_tbl
PARSER_STATUS_t huge gm_ISPSystemRcv(void);//jmp_tbl

//******************************************************************************
//  Debug.c
//******************************************************************************
void huge gm_StopRcvMsg(void);//jmp_tbl
void huge gm_StartRcvMsg(void);//jmp_tbl

//******************************************************************************
//  OtpAccess.c
//******************************************************************************
void huge LoadHdcpKeyArray(BYTE far *Bp_RxArrayAddr, BYTE far *Bp_TxArrayAddr); //jmp_tbl
WORD huge GetRingOscTrimVal(void); //jmp_tbl
BOOL huge gm_OtpSramInit(void); //jmp_tbl
WORD huge gmi_CalculateCrcBlockRam(BYTE* pw_Address, WORD pw_Len); //jmp_tbl
gmt_RET_STAT huge gm_ContProt(BYTE KeyType, void _ss* BKSV); //jmp_tbl
gmt_RET_STAT huge LoadHdcpKey(void); //jmp_tbl

//******************************************************************************
//  CBootUp.c
//******************************************************************************
extern BYTE shift_64Kunit;
void huge gm_SetFlashBankSize(WORD SizeInKB); //jmp_tbl

//******************************************************************************
//  Gm.c
//******************************************************************************
extern FPT_ISR gFp_IsrNonMaskable;

//*******************************************
// Osd.c
//*******************************************
extern WORD OsdSdramChkTimesMax;

//*************************************************************************************************
// DrvLib function
//*************************************************************************************************
#define SYSTEM_TCLK_FREQ    27000000L   //TCLK frequence for Athena, 27.0000MHz
#define SYSTEM_OCLK_FREQ    100000000L  //OCLK frequence, 100 MHz

// OSD
void gm_OsdFixCcTtxTileColorIndex(WORD *Wp_ColorIndex);
void gm_OsdLoadCcTtxSpecificClut(WORD W_StartIndex, WORD W_Size, WORD const far *Wp_ColorData);
void gm_OsdSetBlendLevel(BYTE B_BlendIndex, BYTE B_BlendLevel);
WORD gm_OsdGetHorizontalSize(void);
WORD gm_OsdGetVerticalSize(void);
void gm_OsdHFlipControl(BOOL Bt_State);

// INTERRUPT
#define INTERRUPT_ENABLE(IntMaskRegisterAddr, Mask) gm_SetRegBitsWord(IntMaskRegisterAddr, Mask)
#define INTERRUPT_DISABLE(IntMaskRegisterAddr, Mask) gm_ClearRegBitsWord(IntMaskRegisterAddr, Mask)
gmt_RET_STAT far gm_InterruptSetFunc(gmt_IsrName, FPT_ISR);

// INIT
void far gm_IromInit(void);

// DEBUG
void huge gm_EnablePrints(WORD flag);

//DDC2Bi
void far SetCapabilitiesString(BYTE const far* CapStrStart, WORD CapStrSize);

#endif // __GM_DRVLIB_H__

//*****************************  STANDARD END  *********************************
