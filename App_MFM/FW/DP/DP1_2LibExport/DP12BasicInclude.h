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

#ifndef __DP1_2_BASIC_INCLUDE_H__
#define __DP1_2_BASIC_INCLUDE_H__

//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
//******************************************************************************
//  I N C L U D E   F I L E S
//******************************************************************************

//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************
#pragma option -b- //as a byte
//
// General bit definition
//
#define DP_BIT0					0x0001
#define DP_BIT1					0x0002
#define DP_BIT2					0x0004
#define DP_BIT3					0x0008
#define DP_BIT4					0x0010
#define DP_BIT5					0x0020
#define DP_BIT6					0x0040
#define DP_BIT7					0x0080
#define DP_BIT8					0x0100
#define DP_BIT9					0x0200
#define DP_BIT10					0x0400
#define DP_BIT11					0x0800
#define DP_BIT12					0x1000
#define DP_BIT13					0x2000
#define DP_BIT14					0x4000
#define DP_BIT15					0x8000
#define DP_BIT16					((U32)(1) << (16))
#define DP_BIT17					((U32)(1) << (17))
#define DP_BIT18					((U32)(1) << (18))
#define DP_BIT19					((U32)(1) << (19))
#define DP_BIT20					((U32)(1) << (20))
#define DP_BIT21					((U32)(1) << (21))
#define DP_BIT22					((U32)(1) << (22))
#define DP_BIT23					((U32)(1) << (23))
#define DP_BIT24					((U32)(1) << (24))
#define DP_BIT25					((U32)(1) << (25))
#define DP_BIT26					((U32)(1) << (26))
#define DP_BIT27					((U32)(1) << (27))
#define DP_BIT28					((U32)(1) << (28))
#define DP_BIT29					((U32)(1) << (29))
#define DP_BIT30					((U32)(1) << (30))
#define DP_BIT31					((U32)(1) << (31))

#ifndef NULL
#define NULL					0
#endif

#define FAR						far
#define NEAR					near
#define DP_FALSE					0
#define DP_TRUE					1

#define DP_ROM					const FAR
#define DP_RAM					NEAR 
#define DP_MEM					FAR 
#define DP_EXT_FUNC				FAR
#define DP_INT_FUNC				far
#define EXT_FUNC				far

//******************************************************************************
//  M A C R O   D E F I N E S
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S 
//******************************************************************************
#ifndef DEFINED_U8
#define DEFINED_U8
typedef unsigned char           U8;
#endif

#ifndef DEFINED_U16
#define DEFINED_U16
typedef unsigned short int      U16;

#endif

#ifndef DEFINED_U32
#define DEFINED_U32
typedef unsigned long int		U32;
#endif

/* Common signed types */
#ifndef DEFINED_S8
#define DEFINED_S8
typedef signed char   			S8;
#endif

#ifndef DEFINED_S16
#define DEFINED_S16
typedef signed short int      	S16;
#endif

#ifndef DEFINED_S32
#define DEFINED_S32
typedef signed long int        	S32;
#endif

typedef int     		BOOL;
typedef unsigned long			DP_Addr_t;

//******************************************************************************
//  T Y P E D E F S   ( E N U M S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( F U N C T I O N   P O I N T E R S )
//******************************************************************************

//******************************************************************************
//  T Y P E D E F S   ( S T R U C T S )
//******************************************************************************

//******************************************************************************
//  V A R I A B L E   E X T E R N S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N   E X T E R N S
//******************************************************************************
//******************************************************************************
//  F U N C T I O N   P R O T O T Y P E S
//******************************************************************************
#pragma option -b.
#ifdef __PARADIGM__
#pragma pack(pop)
#endif
#endif		// __DP1_2_BASIC_INCLUDE_H__
