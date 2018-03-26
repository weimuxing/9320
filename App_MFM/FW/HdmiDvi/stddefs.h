/*****************************************************************************/
/* COPYRIGHT (C) 2009 STMicroelectronics - All Rights Reserved               */
/* ST makes no warranty express or implied including but not limited to,     */
/* any warranty of                                                           */
/*                                                                           */
/*   (i)  merchantability or fitness for a particular purpose and/or         */
/*   (ii) requirements, for a particular purpose in relation to the LICENSED */
/*        MATERIALS, which is provided “AS IS”, WITH ALL FAULTS. ST does not */
/*        represent or warrant that the LICENSED MATERIALS provided here     */
/*        under is free of infringement of any third party patents,          */
/*        copyrights,trade secrets or other intellectual property rights.    */
/*        ALL WARRANTIES, CONDITIONS OR OTHER TERMS IMPLIED BY LAW ARE       */
/*        EXCLUDED TO THE FULLEST EXTENT PERMITTED BY LAW                    */
/*                                                                           */
/*****************************************************************************/
/**
 @File   stddefs.h
 @brief Contains a number of generic type declarations and defines.



*/
#ifndef __STDDEFS_H
#define __STDDEFS_H

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
// MODULE:      stddefs.h
//
// USAGE:       Header file for all common global definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#if 1

#if 0
#define ROM const far
#endif

/* Common unsigned types */
#ifndef DEFINED_U8
#define DEFINED_U8
typedef unsigned char  U8;
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
typedef unsigned int U16;
#endif

#ifndef DEFINED_U32
#define DEFINED_U32
typedef unsigned long U32;
#endif

/* Common signed types */
#ifndef DEFINED_S8
#define DEFINED_S8
typedef signed char  S8;
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
typedef signed int S16;
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
typedef signed long   S32;
#endif


#define ROM 	const far
// Common default values
#define NULL_PTR     0

//#define OFF          0
//#define ON           0xFF


// Common Boolean logic
//typedef enum bool_enum
//{
//    FALSE,
//    TRUE
//} BOOL;

#endif
//Pointer type for external (API) library functions
#define EXT_FUNC	far
//Pointer type for data exported from library
#define EXT_DATA	near



/* Common driver error constants */
#define ST_DRIVER_ID   0
#define ST_DRIVER_BASE (ST_DRIVER_ID << 16)
enum
{
   ST_NO_ERROR = ST_DRIVER_BASE,       /* 0 */
   ST_ERROR_BAD_PARAMETER,             /* 1 Bad parameter passed       */
   ST_ERROR_NO_MEMORY,                 /* 2 Memory allocation failed   */
   ST_ERROR_UNKNOWN_DEVICE,            /* 3 Unknown device name        */
   ST_ERROR_ALREADY_INITIALIZED,       /* 4 Device already initialized */
   ST_ERROR_NO_FREE_HANDLES,           /* 5 Cannot open device again   */
   ST_ERROR_OPEN_HANDLE,               /* 6 At least one open handle   */
   ST_ERROR_INVALID_HANDLE,            /* 7 Handle is not valid        */
   ST_ERROR_FEATURE_NOT_SUPPORTED,     /* 8 Feature unavailable        */
   ST_ERROR_INTERRUPT_INSTALL,         /* 9 Interrupt install failed   */
   ST_ERROR_INTERRUPT_UNINSTALL,       /* 10 Interrupt uninstall failed */
   ST_ERROR_TIMEOUT,                   /* 11 Timeout occured            */
   ST_ERROR_DEVICE_BUSY,               /* 12 Device is currently busy   */
   ST_ERROR_SUSPENDED,                  /* 13 Device is in D1 or D2 state */
   DTV_ERROR_NOT_INITIALIZED,
   DTV_ERROR_NO_RESOURCE
};

#if 0
U32 far gm_ReadRegDWord(U16 W_RegAddr);

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

#define gW_HdmiRegBaseAddress 0

// Read or Write to a given register
#define gm_ReadRegByte(W_RegAddr)            \
   (*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr))

#define gm_WriteRegByte(W_RegAddr, RegValue) \
   (*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr) = (BYTE)(RegValue))

#define gm_ReadRegWord(W_RegAddr)            \
   (*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr))

#define gm_WriteRegWord(W_RegAddr, W_RegValue)  \
   (*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr) = (WORD)(W_RegValue))

#define gm_ReadRegTriBytes(W_RegAddr)  \
    (gm_ReadRegDWord(gW_HdmiRegBaseAddress+W_RegAddr) & 0x00ffffffUL)

#define gm_WriteRegTriBytes(W_RegAddr, D_RegValue)                         \
   (*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)   = (WORD)(D_RegValue));  \
   (*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr+2) = (BYTE)(D_RegValue >> 16))

// Set or clear specified bits in given register
#define gm_SetRegBitsWord(W_RegAddr, W_RegValue)   \
   ((*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) | ((WORD volatile)(W_RegValue)))

#define gm_ClearRegBitsWord(W_RegAddr, W_RegValue) \
   ((*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((WORD volatile)(W_RegValue))))

#define gm_SetRegBitsByte(W_RegAddr, W_RegValue)   \
   ((*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) | ((BYTE volatile)(W_RegValue)))

#define gm_ClearRegBitsByte(W_RegAddr, W_RegValue) \
   ((*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((BYTE volatile)(W_RegValue))))

#define gm_ClearAndSetBitsByte(W_RegAddr, B_AndData, B_OrData) \
   ((*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = ((*(BYTE volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((BYTE volatile)(B_AndData)))) | ((BYTE volatile)(B_OrData)))

#define gm_ClearAndSetBitsWord(W_RegAddr, B_AndData, B_OrData) \
	((*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = ((*(WORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((WORD volatile)(B_AndData)))) | ((WORD volatile)(B_OrData)))


#define gm_SetRegBitsDWord(W_RegAddr, W_RegValue) \
    ((*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) | ((DWORD volatile)(W_RegValue)))

#define gm_ClearRegBitsDWord(W_RegAddr, W_RegValue) \
   ((*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = (*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((DWORD volatile)(W_RegValue))))

#define gm_ClearAndSetBitsDWord(W_RegAddr, B_AndData, B_OrData) \
	((*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) = ((*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr)) & (~((DWORD volatile)(B_AndData)))) | ((WORD volatile)(B_OrData)))

#define gm_WriteRegDWord(W_RegAddr, W_RegValue)  \
   (*(DWORD volatile* volatile)(gW_HdmiRegBaseAddress+W_RegAddr) = (DWORD)(W_RegValue))



#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif

typedef struct
{
   gm_AVDdsSrcTypes_t    estVidDds;
   gm_AVDdsSrcTypes_t    estAudDds;
}STHDMIRX_DDSConfigParams_t;

/* Device name type */
#define ST_MAX_DEVICE_NAME 16  /* 15 characters plus '\0' */
typedef char ST_DeviceName_t[ST_MAX_DEVICE_NAME];

/* Function return error code */
typedef U32 ST_ErrorCode_t;

/* Revision structure */
typedef const char far * ST_Revision_t;

/* --- Define the EventID --- */

typedef U32 STEVT_EventID_t;

/* --- Define the handler of an open EH --- */

typedef U32 STEVT_Handle_t;

#define STOS_memcpy(cached_dest, cached_src, size)      gm_SclHdmiMemCpy(cached_dest, cached_src, size)

typedef void * semaphore_t;

//STUBS
//Extern functions
//extern U32 EXT_FUNC gm_GetSystemTime(void);
//extern void EXT_FUNC gm_Delay1us(U16 W_Value);
//extern void EXT_FUNC gm_Print (const char far *Bp_Str, U16 W_Value);
extern BOOL EXT_FUNC gm_SclHdmiMemCpy(U8  * Bp_Dest, U8  * Bp_Source, U16 B_Cnt);

/* STTBX wrapping */
#define STTBX_REPORT_LEVEL_INFO     0
#define STTBX_REPORT_LEVEL_ERROR    1
#define STTBX_REPORT_LEVEL_WARNING  2

#define STTBX_Print(x)    gm_Print((const char far *)x,0)
#define STTBX_Report(x) {}
//void* STOS_MapRegisters(void* pAddr, U32 Length, char * Name)
//{

//   return pAddr;
//}

//void* STOS_UnmapRegisters(void* pAddr, U32 Length)
//{

//   return pAddr;
//}


#define UNUSED_PARAMETER(x)    (void)(x)
#define STOS_TaskDelayUs(x) gm_Delay1us(x)

//#define BYTE U8
//#define WORD U16
//#define DWORD U32

//extern void gm_Delay1ms(WORD W_Value);
//extern DWORD gm_GetSystemTime(void);

#define STOS_time_now() gm_GetSystemTime()
#define STOS_TimeNow() gm_GetSystemTime()
#define STOS_time_minus(t1,t2) ((unsigned long)t1 - (unsigned long)t2)
#define ST_GetClocksPerSecond() gm_GetSystemTime()
#endif // __GLOBAL_H__
//*********************************  END  **************************************

