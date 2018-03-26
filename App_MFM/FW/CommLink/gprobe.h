/*
	$Workfile:   gprobe.h  $
	$Revision: 1.8 $
	$Date: 2012/01/06 03:20:53 $
*/
#define __GPROBE_H__
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
// MODULE:      gProbe.h
//
// USAGE:       Header file for gProbe debug definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//
// gProbe command codes, which are not supported in IROM
//
#ifdef USER_EXTENDED_GPROBE_CMD
#define CMD_SetParam        0x11    // custom command
#define CMD_AppsTest        0x12    // custom command
#endif

// Extended (executed outside IROM) gProbe command codes
#define CMD_WrNvramBlock      0x15    // write NVRAM from RAM buffer
#define CMD_WrNvram           0x16    // write NVRAM from data packet
#define CMD_RdNvram           0x17    // read back NVRAM
#define CMD_DDC2BiPrintPortSelect 0x43 //select DDC2Bi to printout message.

#define CMD_SMTProtocol		0xFE	// Recieve and send SMT messages.

#define CMD_NAK   0x0B
#define CMD_ACK   0x0C
#define CMD_INVALID  0xFF

#define CMD_READ_ID         0xAB     // flash device cmd 
#define CMD_READ_ID2        0x9F     // for ST and MXIC

#define XROM_SIGNATURE_OFFS     0x0000   // location of signature, fixed
#define CONFIG_DATA_OFFSET      0x32     //offset from 0x410000

//******************************************************************************
//         D E B U G   C O M M A N D   S T R U C T U R E S
//******************************************************************************

// Invoke apps debug function - extended debugger command
typedef struct AppsFuncCmdStruct
{
   BYTE	length;				// length of this command packet
   BYTE	cmd;
   BYTE	Index;				// function number
} AppsFuncCmd_t;

// command packet to Read a data from NVRAM
typedef struct ReadNvram
{
   BYTE	len; 				// length of this command packet
   BYTE	cmd;
   BYTE	NvramAddr_hi;
   BYTE	NvramAddr_lo;
   BYTE	length;
   BYTE	chk;
} RdNvramCmd_t;

// command packet to Write RAM buffer in NVRAM
typedef struct WriteNvramBlock
{
   BYTE	len; // length of this command packet
   BYTE	cmd;
   BYTE	RamAddr_hi;
   BYTE	RamAddr_lo;
   BYTE	NvramAddr_hi;
   BYTE	NvramAddr_lo;
   BYTE	Length_hi;
   BYTE	Length_lo;
   BYTE	chk;
} WrNvramBlockCmd_t;

// response packet for read packet command
typedef struct RdCmdPktStruct
{
   BYTE   length;                      // length of this response packet
   BYTE   cmd;                         // response packet type
   BYTE   value[SIO_RxBufLen-3];       // value read from register
   BYTE   chk;                         // checksum of packet
} RdCmdPkt_t;

typedef struct 
{
    WORD ConfigCrc;             // crc of configuration table
    WORD ConfigLength;         // size in bytes of configuration table, code starts immediatly after configuration table
    DWORD FlashSize;             // size in bytes of flash device
    WORD CodeCrc;               // crc of code, crc of code data region (starting immediatly after configuration table
                                    // and ending at end of flash (determine by flashsize
    BYTE fw_major;             // major relese number of application (1 or 1.23.156)
    BYTE fw_minor;             // minor release number (23 of 1.23.156)
    WORD fw_build;             // build number (156 of 1.23.156)
} TXromConfigData;

//******************************************************************************
//  G L O B A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
PARSER_STATUS_t SerialCmdHandler	(void);
void GprobeCmdParser(BYTE far* pBuf, HANDLER_USE_t B_whichHandler);

#ifdef USER_EXTENDED_GPROBE_CMD
void  SetParam				(void);
DWORD GetParam				(BYTE B_Index);
void UserExtendableCmd(HANDLER_USE_t B_whichHandler, BYTE B_TestNum);
#endif
//*********************************  END  **************************************
