/*
	$Workfile:   gm_OsdAdjustEngine.h  $
	$Revision: 1.2 $
	$Date: 2011/12/14 09:20:57 $
*/
#ifndef __OSD_AE_H__
#define __OSD_AE_H__
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
// MODULE:      osd_ae.h
//
// USAGE:
//
//******************************************************************************

//******************************************************************************
//  T Y P E    D E F I N I T I O N S
//******************************************************************************

typedef void	(*FPT_AdjustFunc)		(void);
typedef BYTE	(*FPT_ByteMinMaxFunc)	(void);
typedef SBYTE	(*FPT_SByteMinMaxFunc)	(void);
typedef WORD	(*FPT_WordMinMaxFunc)	(void);
typedef SWORD	(*FPT_SWordMinMaxFunc)	(void);
typedef WORD	(*FPT_StepFunc)			(BYTE);

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_OSD_ADJUSTER_PROPERTIES_ENTRY_STRUCT
{
	BYTE	B_Ctrl;
	WORD	W_Offset;
	BYTE	B_Step;
	BYTE	B_Id;
	WORD	W_Min;
	WORD	W_Max;
} ST_OSD_ADJUSTER_PROPERTIES_ENTRY;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_OSD_ADJUSTER_PROPERTIES_ENTRY_STRUCT_AE4		// WB4
{
	BYTE	B_Ctrl;
	BYTE	B_StructId;
	WORD	W_Offset;
	BYTE	B_Step;
	BYTE	B_Id;
	WORD	W_Min;
	WORD	W_Max;
} ST_OSD_ADJUSTER_PROPERTIES_ENTRY_AE4;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

#ifdef __PARADIGM__
#pragma pack(push)
#pragma pack(1)
#endif //__PARADIGM__

typedef struct ST_OSD_ADJUSTER_PROPERTIES_ENTRY_STRUCT_AE5		// WB4
{
	BYTE	B_Ctrl;
	BYTE	B_StructId;
	WORD	W_Offset;
	BYTE	B_Step;
	WORD	W_Id;
	WORD	W_Min;
	WORD	W_Max;
} ST_OSD_ADJUSTER_PROPERTIES_ENTRY_AE5;

#ifdef __PARADIGM__
#pragma pack(pop)
#endif //__PARADIGM__

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define GET_ADJ 2
#define INC_ADJ 0
#define DEC_ADJ 1

#define VT_MASK_TYPE		(BIT0 | BIT1)
#define VT_MASK_MIN			(BIT5)
#define VT_MASK_MAX			(BIT6)
#define VT_GROUP_IND(val)	((val >> 2) & (BIT0 | BIT1 | BIT2))

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void IncDecAdjuster(BYTE B_Command);
void IncDecAdjusterSM4(WORD W_Offset, BYTE B_Command);
void SetAdjusterValue(WORD W_Offset, WORD W_Value);
SDWORD GetIncDecAdjusterValue(WORD W_Offset, BYTE B_Command);
SDWORD GetAdjusterMin(WORD W_Offset);
SDWORD GetAdjusterMax(WORD W_Offset);

//******************************************************************************
//  C O D E
//******************************************************************************

#endif // __OSD_AE__
