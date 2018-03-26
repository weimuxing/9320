//******************************************************************
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
//================================================================
//
// MODULE:	flash.h
//
// USAGE :	Standard low level Flash read/write
//          The driver classifies the parallel flash device by the
//          data width interface.  Two group of device are defined:
//          GROUP_DEVICE_x8 - these are flash devices with access
//                            by byte.
//          GROUP_DEVICE_x16 - these are flash devices with access
//                            by byte.
//
//  Notes:
//    1. The group definition is to be defined in project file
//    2. The __ADDR24__ is enabled/disabled in the IDE file
//
//******************************************************************

#ifndef __FLASH_H__
#define __FLASH_H__



//**************************************************************
//					 D E F I N I T I O N S
//**************************************************************
#ifndef __ADDR24__
#define TRANSFORM_ABS_ADDR(abs_addr) ( ((((DWORD)abs_addr) & 0xf0000L)<<12) +  ((DWORD)abs_addr  & 0xFFFF) )
#define HUGE_ADDR(hi,med,lo)         ((((DWORD)hi)<<28) + ((((WORD)med)<<8) + (lo)))
#else
#define TRANSFORM_ABS_ADDR(abs_addr) ( ((((DWORD)abs_addr) & 0xff0000L)<<8) +  ((DWORD)abs_addr  & 0xFFFF) )
#define HUGE_ADDR(hi,med,lo)         ((((DWORD)hi)<<24) + ((((WORD)med)<<8) + (lo)))
#endif

#define  BYTE_ACCESS       0
#define  WORD_ACCESS       1

#define PROTECT_HI_BYTE		0xFF00
#define PROTECT_LO_BYTE		0x00FF

#define FlashReadByte(Addr)		(BYTE)(*(BYTE far *)TRANSFORM_ABS_ADDR(Addr))
#define FlashReadWord(Addr)		(WORD)(*(WORD far *)TRANSFORM_ABS_ADDR(Addr))

//*************************************************************************
//*************************************************************************
typedef enum SECTORERASEMODE
{
   SECT_ER_ALL = 0,            //Erase all sectors of chip
   SECT_ER_DEST_INCLUDED,      //Sector Erase Destination Address Included
   SECT_ER_DEST_EXCLUDED       //Sector Erase Destination Address ExIncluded
}SECTORERASEMODE;


typedef struct tag_FlashDriver
{
   BOOL (*Write)(BYTE B_Data);
   BOOL (*PageProgram)(DWORD Dw_Addr, BYTE * pB_Data, WORD W_Length);
   BOOL (*Read)(BYTE * B_RD_Data);
   BOOL (*WriteProtectionDisable)(void);
   BOOL (*WriteProtectionEnable)(void);
   BOOL (*WriteEnable)(void);
   BOOL (*ReadStatusRegister)(BYTE * B_StatusReg);
   BOOL (*StatusPoll)(WORD W_TimeOut);
   BOOL (*ReadBurst)(DWORD Dw_StartAddr, BYTE B_Length, BYTE * Ba_Buf);
   BOOL (*ChipErase)(void);
   BOOL (*SectorErase)(DWORD Dw_Addr);
} FLASHDRIVER;


typedef struct tag_FlashCommands
{
   BYTE WREN;			/* write enable */
   BYTE BYTE_PROGRAM;	/* program bybte */
   BYTE WRDI;			/* WRITE disable command*/
   BYTE RDSR; 			/* read status command*/
   BYTE EWSR; 			/* enable write status register command*/
   BYTE WRSR;			/* write status register command*/
   BYTE HS_READ; 		/* High speed read command*/
   BYTE ERASE;			/* Chip erase command*/
   BYTE SECT_ERASE;  /* Sector erase command*/
} FLASHCOMMANDS;

typedef struct chipExeption
{
   BYTE B_MID; //manufacturer ID
   BYTE B_DID; //chip ID
   DWORD DW_Size; // chip memory size in bytes
   WORD W_Page; //writing page size in bytes
   WORD W_SectorSize;
   FLASHDRIVER *Driver;
   FLASHCOMMANDS *Commands;
}ChipFeatures_t, * pChipFeatures_t;


typedef struct flashInfo
{
   BYTE B_MID; //manufacturer ID
   BYTE B_DID; //chip ID
   DWORD DW_Size; // chip memory size in bytes
   WORD W_Page; //writing page size in bytes
   WORD W_SectorSize;
}FlashInfo_t;

#define SEQUOIA_SPI_CLK_DIVIDER  5	/* sequoia has a different clk divider*/
#define ATEHNA_SPI_CLK_DIVIDER   6	/* sequoia has a different clk divider*/

/*SPI flash memory commands*/
#define SPI_READ_ID_1            0x90		 /* read id command method 1*/
#define SPI_READ_ID_2            0x9F		 /* read id command method 2*/

#define PROGRAM_TIMEOUT          4000	/* *10*ms */
#define ERASE_TIMEOUT            6000 	//Erase operation duration in miliseconds for general case
#define SECT_ERASE_TIMEOUT       10	  	/* *10*ms */
#define WRITE_TIMEOUT            2	  	/* *10*ms */
#define ACCESS_TIMEOUT           2	  	/* *1*ms */
#define SPI_CLK_DIVIDER          3		/* 2,3,4 are supported */

#define FLASH_SIZE               (2*1024UL*1024UL)  /* in # of bytes*/
#define PAGE_SIZE                1  /* in # of bytes*/

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************



//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BOOL Flash_EraseBlockX			(void);
BOOL Flash_WriteBlockX			(void);
BOOL Flash_EraseSector        (DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode);
BOOL SerialFlashInit	         (void);
BOOL WriteBuf2Flash           (DWORD StartAddr,WORD length, BYTE * p_Buf);
BOOL EnableWriteProtection    (void);
BOOL ClearWriteProtection     (void);
void GetSpiFlashInfo          (FlashInfo_t * pInfo);
BYTE FlashWriteByte(DWORD DestAddr, BYTE Data);
BYTE FlashWriteWord(DWORD DestAddr, WORD Data);

// Commonly used functions
WORD MakeCRC16                (BYTE * buffer, WORD length);
WORD GenCRC16                 (WORD Accum, BYTE DataIn);
DWORD CalcSerFlashMemAddr     (BYTE* Bp_Pointer);
#endif	// __FLASH_H__
//******************************************************
//******************************************************
