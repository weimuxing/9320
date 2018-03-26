/*
   $Workfile:   smt_Filters.h  $
   $Revision: 1.3 $
   $Date: 2011/12/15 03:52:11 $
*/

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
// MODULE:      smt_Filters.h
//
//******************************************************************
#ifndef SMT_FILTERS_INCLUDED
#define SMT_FILTERS_INCLUDED

#ifdef USE_SMT

typedef enum Filters_CommandEnum
{
   Filters_Write=1,
   Filters_Read=2,
} Filters_CommandType;

typedef enum
{
   Filters_Horis = 0,
   Filters_Vert = 1
}Filters_DirectionType;

typedef enum
{
   Filters_Main = 0,
   Filters_Pip = 1
}Filters_PathType;

typedef enum
{
   Filters_Y = 0,
   Filters_UV = 1
}Filters_ChannelType;

typedef enum
{
   Filters_LPF = 0,
   Filters_HPF = 1,
   Filters_COMB = 2
}Filters_ResponceType;

#pragma option -b-
typedef enum
{
   Filters_OK = 0,
   Filters_GeneralError = 1,
   Filters_InvalidParameter = 2
}Filters_ErrorIdType;
#pragma option -b.

typedef struct
{
   Filters_CommandType ComID;			//< command id (16bit unsigned) >,
   Filters_DirectionType Dir;			//<Filt_Direction_value (16bit unsigned) >,			 // 0 = Horizontal,  1 = Vertical
   Filters_PathType Path;				//< Channel_Path_value (16bit unsigned) >,			// 0 = Main, 1 = PIP
   Filters_ChannelType Channel;			//<Y_or_UV_Channel_value (16bit unsigned) >,	 // 0=Luminance, 1=Chrominance
   Filters_ResponceType RespType;			//<RespType_value (16bit unsigned) >,   // 0=LPF, 1=HPF, 2=Combined (LPF + HPF)
   BYTE  Boosting;					//<Boosting value <8bit signed>,
   BYTE  Reserved[7];					//<Unused values (7 * 8bit)	// for future extensions
   WORD Length;						//<Data_Length (16bit unsigned) >,						// Data size
   WORD Data[1];						//<Data (Data_Length * 16 bit unsignes)>

}Filters_WriteType;

typedef struct
{
   Filters_CommandType ComID;				//< command id(16bit unsigned) >
   Filters_DirectionType Dir;			//<Filt_Direction_value (16bit unsigned) >,			 // 0 = Horizontal,  1 = Vertical
   Filters_PathType Path;				//< Channel_Path_value (16bit unsigned) >,			// 0 = Main, 1 = PIP
   Filters_ChannelType Channel;			//<Y_or_UV_Channel_value (16bit unsigned) >,	 // 0=Luminance, 1=Chrominance
   Filters_ResponceType RespType;			//<RespType_value (16bit unsigned) >,   // 0=LPF, 1=HPF, 2=Combined (LPF + HPF)
}Filters_ReadRequestType;


#pragma pack(push)
#pragma pack(1)
typedef struct
{
   Filters_ErrorIdType ErrID;   //8bit unsigned
   WORD Length;			     //<Data_Length (16bit unsigned) >,
   WORD Data[1];			     //<Data (Data_Length * 16 bit unsignes)>
}Filters_ReadResponseType;
#pragma pack(pop)

// type is using to get automatic convertion of filter type selectors from external tool to offset table index.
typedef union filtertypesunion
{
   WORD type;
   struct
   {
      WORD Dir:1;
      WORD Path:1;
      WORD Channel:1;
      WORD RespType:2;
   }sel;
}FilterType;

typedef struct
{
   WORD Shift;
   FilterType Filter;
   BYTE DataStored;
   WORD CurrentCommand:2;
   WORD Channel:2;
   WORD StoredFlag:1;
   WORD ExtraData:1;
   WORD err:1;
}Filters_ContextType;

/**************************************************************************
 Definition for SMT transactions handler to determine buffersize enough for this prot handler
**************************************************************************/
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE sizeof(Filters_ContextType)
#elif SMT_MAX_BUF_SIZE < sizeof(Filters_ContextType)
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE sizeof(Filters_ContextType)
#endif
/**************************************************************************/


typedef struct
{
   BYTE chip;
   WORD offs;
   WORD size;
   BYTE tab;
   WORD boost;
}t_FilterAccesData;

//******************************************************************************/
//  G L O B A L    F U N C T I O N
//******************************************************************************

BYTE Filters_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif

