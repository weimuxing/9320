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

#ifndef __HDCPRPTR_BASIC_INCLUDE_H__
#define __HDCPRPTR_BASIC_INCLUDE_H__

//******************************************************************************
//	C O M P I L E R    O P T I O N S										
//******************************************************************************
// Force byte alignment
#ifdef __PARADIGM__
#pragma pack(push, 1)
#endif
#pragma option -b- //as a byte
//******************************************************************************
//  I N C L U D E   F I L E S
//******************************************************************************

//******************************************************************************
//  C O N S T A N T   D E F I N E S
//******************************************************************************


#ifndef NULL
#define NULL	0
#endif

#define FAR		        far
#define NEAR			near
#define FALSE			0
#define TRUE			1

#define HDCP_ROM		const FAR
#define HDCP_EXT_FUNC	FAR
#define HDCP_INT_FUNC	far
#define EXT_FUNC		far

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

typedef int     		        BOOL;

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
