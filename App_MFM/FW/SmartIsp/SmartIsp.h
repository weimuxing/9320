/*
	$Workfile:   SmartIsp.h  $
	$Revision: 1.17 $
	$Date	 :     $
*/

/****************************************************************************/
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
/*==========================================================================*/

//Revision: 4.1      2011-DEC-20


#ifndef __SMART_ISP_H__
#define __SMART_ISP_H__

typedef enum IspStepID
{
   ISP_STEP_NONE=0,
   ISP_STEP_INIT,
   ISP_STEP_SEARCH_PIC,
   ISP_STEP_EXTRACT_PICINFO,
   ISP_STEP_EXTRACT_HEXBINARY,
   ISP_STEP_VERIFY_CHECKSUM,
   ISP_STEP_VERIFY_SIGNATURE,
   ISP_STEP_GET_CONFIRMATION,
   ISP_STEP_FLASH_ERASE,
   ISP_STEP_FLASH_WRITE,
   ISP_STEP_FINISHED
}IspStepID;

typedef enum IspStepStatusID
{
   ISP_SSTATUS_NONE=0,
   ISP_SSTATUS_PROCESSING,
   ISP_SSTATUS_FAILED,
   ISP_SSTATUS_SUCCESS
}IspStepStatusID;

typedef enum IspActionConfirmStatus
{
   ISP_ACTION_UNCONFIRMED=0,
   ISP_ACTION_CONFIRMED,
   ISP_ACTION_REJECTED
}IspActionConfirmStatus;

typedef enum IspDetHdlrState
{
   ISP_DETHDLR_STOP=0,
   ISP_DETHDLR_RESET,
   ISP_DETHDLR_START,
   ISP_DETHDLR_SEARCH_PREFIX,
   ISP_DETHDLR_SEARCH_PATTERN,
   ISP_DETHDLR_PATTERN_MATCH
}IspDetHdlrState;

typedef enum IspExecuteMode
{
   ISP_EXECUTE_KEYPAD,
   ISP_EXECUTE_DIRECT,
   ISP_EXECUTE_AUTO_DET
}IspExecuteMode;

typedef enum 
{
   SISP_DUALFLASH_DISABLE,
   SISP_DUALFLASH_ENABLE_NORMAL,
   SISP_DUALFLASH_ENABLE_PATCH_MODE,
   END_OF_SISP_DUALFLASH_MODE
}SIspDualFlashMode;

#define BURST_SIZE                  64
#define BURST_SIZE_SHIFT            6
#define PIX_BUF_SIZE                720
#define SigLen                      7
#define MaxLocNo                    16
#define BIN_BUF_SIZE                1920


//#define VERMAJOR_OFFSET 0
//#define VERSUB1_OFFSET  1
//#define VERSUB2_OFFSET  2
//#define VERSUB3_OFFSET	3
//#define VERSION_LEN		4
//Firmware Version is defined in C0starup.asm
//#define FW_SIG_OFFSET 	4
#define FW_SIG_LEN    	4
#define BIT_PER_BYTE          8


typedef struct LocRecordStruct
{
   DWORD LocAddr;
   DWORD LocLength;
}LocRecordStruct;

typedef struct IspInfoStruct
{
   WORD	FrameWidth;
   WORD	FrameHeight;
   DWORD	FrameBufStart;
   DWORD	FmBinBufStart;
   BYTE	BurstAdjust;
   WORD	PicInCordx;
   WORD	PicInCordy;
   WORD	PicWidth;
   WORD	PicHeight;
   BYTE	TotalPicNum;			//Add for Lib 2.0
   BYTE	CurentPicNum;		//Add for Lib 2.0
   BYTE	LLNo;
   BYTE	Is_PicCordx_InOdd;
   LocRecordStruct	Loc[MaxLocNo];
   DWORD	FwBinTotal;
//	BYTE  Ver_Major;
//	BYTE  Ver_Sub1;
//	BYTE  Ver_Sub2;
//	BYTE  Ver_Sub3;
   void (EXT_FUNC* OsdWirtePercentage)(DWORD sizeTotal, DWORD sizeWrited);
   BYTE  BitsPerPix;
   DWORD SdramSize;
}IspInfoStruct;



typedef struct PixRGB_Struct
{
   BYTE b_Red;
   BYTE b_Grn;
   BYTE b_Blu;
}PixRGB_Struct;


typedef enum IspFrameCatchCmd
{
   FCATCH_START,
   FCATCH_RESTART,
   FCATCH_END
}IspFrameCatchCmd;

typedef enum PrintPixelDataMode
{
   PRINT_BY_IN_DEPTH,
   PRINT_8BIT,
   PRINT_10BIT,
   PRINT_YUV_20BIT,
   PRINT_HI_NIBBLE_COMBINE,
   PRINT_RAW
}PrintPixelDataMode;

#ifdef ST_4K2K_93xx_BOARD
typedef enum IspSlaveModeCmd
{
   ISP_SLAVE_CMD_NONE         = 0,
   ISP_SLAVE_CMD_RUN_DIRECT   = BIT0,
   ISP_SLAVE_CMD_RUN_KEYPAD   = BIT1,
   ISP_SLAVE_CMD_ERASE_START  = BIT2,
   ISP_SLAVE_CMD_ERASE_REJECT = BIT3,
   ISP_SLAVE_CMD_FAIL_EXIT    = BIT4,
   ISP_SLAVE_CMD_NORMAL_EXIT  = BIT5,
   ISP_SLAVE_CMD_HARD_RESET   = BIT6   
}IspSlaveModeCmd;
#endif

typedef struct SmartIspFlashDriver
{
   BOOL (*FlashInit)(void);
   BOOL (*FlashErSectr)(DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode);
   BOOL (*WrBuf2Flash)(DWORD StartAddr,WORD length, BYTE * p_Buf);
   void (*GetFlashInfo)(FlashInfo_t * pInfo);   
} SmartIspFlashDriver;

BOOL IspThruDisplay(BYTE ExeMode);
void ReadExtSdram2Buf (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD far * Wa_ReadData);
void WriteBuf2ExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD far * Wa_WriteData);
void PrintPicLine(WORD line, BYTE Ptype);
BYTE GetIspStep(void);
BYTE GetIspStepStatus(void);
void SetIspActionConfirm(BYTE answer);
BYTE GetIspActionConfirm(void);
void SysteHardReset(void);

#ifdef ST_4K2K_93xx_BOARD
void SmartIspSlaveControl(BYTE Control);
void SmartIspSlaveHandler(void);
#endif


//Below Function is from SmartIspLib
void Print_SmartIspLibVersion(void);
BOOL Isp_PicSearch(IspInfoStruct* InfoPtr, BYTE* fBuf);
BOOL Isp_ExtractPicL0(IspInfoStruct* InfoPtr, BYTE* fBuf, BYTE* PLbuf);
BOOL Isp_ExtractBin(IspInfoStruct* InfoPtr, BYTE* fBuf, BYTE* PLbuf);
BOOL Isp_VerifyBin(IspInfoStruct* InfoPtr, BYTE* PLbuf);
BOOL Isp_VerifySig_GetVer(IspInfoStruct* InfoPtr, BYTE* PLbuf, BYTE VerLen);
BOOL Isp_FlashErase(DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode, BYTE DualFlashMode);
BOOL Isp_FlashWrite(IspInfoStruct* InfoPtr, BYTE* PLbuf);
DWORD Isp_FrameCatch(IspInfoStruct* InfoPtr, BYTE Cmd);
void SmartIspDetectHandler(void);
void EnableSmartIspDetect(BOOL b_En);
BOOL IsIspAutoDetMode(void);
void SetIspAutoDetMode(BOOL b_Set);
BOOL PrintDisplayPixelData(IspInfoStruct* InfoPtr,BYTE PrnMode, WORD PosX, WORD PosY, WORD RdLen,BYTE* fBuf);
BOOL Isp_LoadFlashDrv(SmartIspFlashDriver* FlashDrvPtr);

#endif



