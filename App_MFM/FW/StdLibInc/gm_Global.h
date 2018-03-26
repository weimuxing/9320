/*
	$Workfile:   gm_Global.h  $
	$Revision: 1.8 $
	$Date: 2011/12/13 10:32:44 $
*/
#ifndef __GM_GLOBAL_H__
#define __GM_GLOBAL_H__
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
// MODULE:      gm_Global.h
//
// USAGE:       Header file for all common global definition
//
//******************************************************************************

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//
// Common data type cast
//
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef signed char SBYTE;
typedef signed int SWORD;
typedef signed long SDWORD;

#define BYTE_SIZE	sizeof(BYTE)
#define WORD_SIZE	sizeof(WORD)

//
// Define variable with const far will place the variable in ROM area.
//
#define ROM 	const far
#define code 	const far 	//data in code segments
#define stack 	_ss 		//local variable assumed to be in stack
#define ndata 	near 		//global variable in 0 segment
#define fdata 	far 		//universal common pointer for data segent(s)
#define hostreg near 		//host register pointer
#define pcbreg 	near 		//intel core register pointer

//
// Data structure for WORD union
//
typedef struct
{
   DWORD	Size;         // Indicates the size of the entire data configuration
   DWORD	Signature;     // This is a unique identificator of the type of the data configuration table.
   DWORD	Version;     // Version of the data
} LLD_TuningData_t;


typedef struct			// Structure for WORD
{
   BYTE B_Lower;		// Intel 186 memory specific
   BYTE B_Upper;		//
} ST_WORD;

typedef union			// Union for WORD with upper and lower byte
{
   WORD    W_Data;
   ST_WORD	St_Data;
} UNION_WORD;


typedef int BOOL;
#define FALSE	0
#define TRUE	1
#define gmd_FALSE FALSE
#define gmd_TRUE TRUE

/*
typedef enum bool_enum
{
	FALSE,
    gmd_FALSE = FALSE,
	TRUE,
	gmd_TRUE = TRUE
} BOOL;*/


#define Off			FALSE
#define On			TRUE

#define DISABLE		0
#define ENABLE		1

//
// Common default value
//
#define NULL_PTR	0

//
// General bit definition
//
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000
#define BIT16       0x00010000UL
#define BIT17       0x00020000UL
#define BIT18       0x00040000UL
#define BIT19       0x00080000UL
#define BIT20       0x00100000UL
#define BIT21       0x00200000UL
#define BIT22       0x00400000UL
#define BIT23       0x00800000UL
#define BIT24       0x01000000UL
#define BIT25       0x02000000UL
#define BIT26       0x04000000UL
#define BIT27       0x08000000UL
#define BIT28       0x10000000UL
#define BIT29       0x20000000UL
#define BIT30       0x40000000UL
#define BIT31       0x80000000UL

typedef BYTE uint8_t;
typedef SBYTE int8_t;
typedef WORD uint16_t;
typedef DWORD uint32_t;
typedef SWORD int16_t;
typedef SDWORD int32_t;
typedef SDWORD int64_t;
typedef BOOL Boolean;
typedef BOOL Bool;

//******************************************************************************
//  HDMI
//******************************************************************************
typedef unsigned int   gmt_REG_ADDR;
typedef unsigned int   gmt_HDMI_REG_ADDR;
typedef unsigned long gmt_HDMI_PACKET_ADDR;

//Pointer type for external (API) library functions
#define HDMI_EXT_FUNC	far

//Pointer type for external functions (gm_Print)
#define EXT_FUNC		far

//Type of external constant data
#define EXT_ROM         const far

//Pointer type for RAM data
#define HDMI_ROM		const far

//Pointer type for RAM data
#define HDMI_RAM		near

//Pointer type to address any memory type either HDMI_ROM or HDMI_RAM
#define HDMI_MEM	    far

//Pointer type for external (API) CEC library functions
#define CEC_EXT_FUNC	far//near //far

//Pointer type for ROM data
#define CEC_ROM			const far

//Pointer type for RAM data
#define CEC_RAM         near

// DPRX/DPTX pointer/function qualifiers
/*
#define DP_EXT_FUNC     far
#define EXT_FUNC        far
#define DP_ROM          const far
#define DP_RAM          near
#define DP_MEM          far
*/

typedef unsigned int    gmt_DP_REG_ADDR;
typedef unsigned long   gmt_DP_DPCD_ADDR;
typedef unsigned long   gmt_DP_SDP_ADDR;

#if 0								// Disable for XROM
//
// Common error number for function return status
//
typedef enum
{
   // Error status
   OK,								// Function Call OK
   ERR_SEND,                   	// Error in sending data
   ERR_RCV,                        // Error in receiving data
   ERR_TMO,                        // Timeout error
   ERR_PARAM,                      // Error in input parameters
   ERR_READ,                       // Error in reading
   ERR_WRITE,                      // Error in writing
   ERR_COMMAND,					// API command failed
   ERR_POINTER,					// Pointer not intialized
   ERR_OVERFLOW,					// Input parameter vlaues over limit
   ERR_MEMORY,						// Out of SDRAM memory
   ERR_NO_DATA,					// IR FIFO queue is empty
   ERR_BUSY,						// Device busy
   OSD_INVALID_TILESET,			// Invalid tileset
   OSD_INVALID_TILE,				// Invalid tile number
   OSD_DISABLE_TILE,				// tile is disabled, can't draw the control
   OSD_INVALID_SIZE,				// Invalid control size
   OSD_INVALID_BPP,				// Invalid color depth
   OSD_INVALID_FONT_ID,			// Invalid font id
   OSD_TOOBIG_STRING,				// Temporary buffer size is too small for rendering
   ERR_LIMIT,						// Reach limit of position
   ERR_ADC_CALIBRATE				// Error in ADC calibration
} RET_STAT, gmt_RET_STAT;
#endif

//******************************************************************************
//  M A C R O   D E F I N I T I O N
//******************************************************************************

#define SETBIT(VAR,BIT)         ( (VAR) = (VAR) | (BIT) )
#define CLRBIT(VAR,BIT)         ( (VAR) = (VAR) & (~(BIT)) )
#define CHECKBIT(VAR,BIT)       ( (VAR) & (BIT) )
#define RETBIT(VAR,BIT)         (((VAR) & (BIT) ) != 0 )
#define	EXTRACT_2B(Cnst)		(BYTE)(Cnst), (BYTE) ((Cnst) >> 8)

#endif
//*********************************  END  **************************************
