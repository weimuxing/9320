/*
   $Workfile:   smt_GammaCorrection.h  $
   $Revision: 1.13 $
   $Date: 2013/04/29 03:17:00 $
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
// MODULE:      smt_GammaCorrection.h
//
//******************************************************************
#ifndef GAMMA_INCLUDED
#define GAMMA_INCLUDED

#ifdef USE_SMT

#define VERSION_0   0
#define VERSION_1   1
#define VERSION_2   2
#define VERSION_3   3
#define VERSION_4   4
#define VERSION_5   5
#define VERSION_6   6

#define GAMMA_SMT_PROTOCOL_VER      VERSION_5   //5 // 4

#pragma option -b-
typedef enum Gamma_CommandEnum
{
   Gamma_StartCalibration=0,
   Gamma_EndCalibration=1,
   Gamma_GetCapabilities=2,
   Gamma_GetColorSpaceCapabilities=3,
   Gamma_DisplayColor=4,
   Gamma_DownloadGammaTable=5,
   Gamma_UploadGammaTable=6,
   //gamma v.2
   Gamma_GetSupportedTables=7,
   Gamma_GetPreLUTColorSpaceCapabilities=8,
   Gamma_DownloadPreLUT=9,
   Gamma_UploadPreLUT=10,
   Gamma_GetColorMappingGamutMatrixColorSpaceCapabilities=11,
   Gamma_DownloadColorMappingGamutMatrix=12,
   Gamma_UploadColorMappingGamutMatrix=13,
   // gamma v3 and v4
   Gamma_DownloadCompressedTable=14,
   Gamma_UploadCompressedTable=15,
   Gamma_GetSteepTableColorspaceCapabilities=16, // 0x10
   Gamma_DownloadLUTAndSteepTable=17, // 0x11
   Gamma_UploadLUTAndSteepTable=18, // 0x12
   Gamma_EnableTable=19, // 0x13
	// gamma v5
   Gamma_GetNumOfProfiles = 20, // 0x14 // version 5
   Gamma_SelectProfile = 21, // 0x15 // version 5
   Gamma_SaveProfile = 22, // 0x16 // version 5
   Gamma_ApplyProfile = 23, // 0x17 // version 5
   Gamma_EraseProfile = 24, // 0x18 // version 5
	#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
   Gamma_HideColor = 25, // 0x19 // version 6
   Gamma_DownloadNativeColorMappingGamutMatrix = 26, // 0x1A // version 6
   Gamma_UploadNativeColorMappingGamutMatrix = 27, // 0x1B // version 6
	#endif
} GammaCorr_CommandType;

#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
    typedef enum
    {
       Gamma_HideColor_OK          =   0,
       Gamma_HideColor_Failure      =   1
    }GamaHideColorRespCode;
#endif

typedef enum
{
   Gamma_Mono_Component=0,
   Gamma_RGB_Component=1,
   Gamma_G_Component=2,
   Gamma_B_Component=3,
   Gamma_R_Component=4
}GamaColorComponentType;

typedef enum
{
   Gamma_Space_RGB=1,
   Gamma_Space_YUV=2,
   Gamma_Space_YPbPr=4,
   Gamma_Space_YCbCr=8,
   Gamma_Space_HSV=16,
   Gamma_Space_HSL=32,
   Gamma_Space_Monochrome=64
}GamaColorSpaceType;


typedef enum
{
   Gamma_OK=0,
   Gamma_ColorSpaceNotSupported=1,
   Gamma_FormatNotSupported=2,
   Gamma_TableIsEmpty = 3,
   Gamma_LUTCompressionNotSupported = 3,
   Gamma_SteepTableCompressionNotSupported = 4,
   Gamma_InconsistentSteepSteps = 5
}GammaReturnCodeType;


typedef enum
{
   Gamma_CompensationNotSupported=0,
   Gamma_SeparateTables=1,
   Gamma_CommonTable=2,
}GammaSupportType;

//gamma v.2
typedef enum SMTGetCapabilities
{
   SMTCapabilities_PostLut_Support = BIT0,
   SMTCapabilities_PreLut_Support = BIT1,
   SMTCapabilities_3x3Matrix = BIT2,
   SMTCapabilities_Compressed_PostLUT = BIT3, // version 3
   SMTCapabilities_Compressed_PreLUT = BIT4, // version 3
   SMTCapabilities_PostLUT_Steep = BIT5, // version 4
   SMTCapabilities_PreLUT_Steep = BIT6, // version 4
   SMTCapabilities_DualChannel = BIT7, // version 4
   SMTCapabilities_Load_Clear_Save = BIT8, // version 5
	#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6) 
   SMTCapabilities_NativeMatrixDataFormat = BIT9    //version 6
	#endif   
}gmt_SMTGetCapabilities;

typedef enum SMTColor_Space
{
   SMTColor_Space_RGB = BIT0,
   SMTColor_Space_YUV = BIT1,
   SMTColor_Space_YPbPr = BIT2,
   SMTColor_Space_YCbCr = BIT3,
   SMTColor_Space_HSV = BIT4,
   SMTColor_Space_HSL = BIT5,
   SMTColor_Space_Monochrome = BIT6,
}gmt_SMTColor_Space;

typedef enum SMTSupport_level
{
   SMTSupport_level_Not_Supported = 0,
   SMTSupport_level_Supported_as_Separate_table = 1,
   SMTSupport_level_Supported_as_Common_table = 2,
}gmt_SMTSupport_level;

typedef enum SMTColor_component
{
   SMTColor_component_Monochrome = 0,
   SMTColor_component_RGBHSLHSVYUVYPbPrYCbCr = 1,
   SMTColor_component_GYH = 2,
   SMTColor_component_BUSPbCb = 3,
   SMTColor_component_RVLPrCr = 4,
}gmt_SMTColor_component;

typedef enum SMTStatus
{
   SMTStatus_OK = 0,
   SMTStatus_Color_Space_Not_Supported = 1,
   SMTStatus_Format_Not_Supported = 2,
   SMTStatus_Table_Not_Available = 3,
}gmt_SMTStatus;
//========================================================//gamma v.2

// gamma v3 and v4
typedef enum
{
   Gamma_PostLUT=0,
   Gamma_PreLUT=1,
}GammaTableType_t;

#pragma option -b.


#pragma pack(push)
#pragma pack(1)
typedef struct GammaCapabilitiesStruct
{
   BYTE   PanelRedWidth;      //   Panel red width   1   Width of the red data path in the panel in bits
   BYTE   PanelGreenWidth;   //   Panel green width   1   Width of the green data path in the panel in bits
   BYTE   PanelBlueWidth;      //   Panel Blue width   1   Width of the blue data path in the panel in bits
   DWORD   PanelRedShades;      //   Panel red shades   4   Number of shades of red the panel is capable of displaying including dithering
   DWORD   PanelGreenShades;   //   Panel green shades   4   Number of shades of green the panel is capable of displaying including dithering
   DWORD   PanelBlueShades;   //   Panel blue shades   4   Number of shades of blue the panel is capable of displaying including dithering
   DWORD   PanelGreyShades;   //   Panel grey shades   4   Number of shades of grey the panel is capable of displaying including dithering
   GamaColorSpaceType   ColorSpace;         //   Color space   1   Operating color spaces (can be ORed):
   //1 - RGB
   //2 - YUV (jpeg format, 3x8 bits, 0..255)
   //4 - YPbPr
   //8 - YCbCr
   //16 - HSV
   //32 - HSL
   //64 - Monochrome
   BYTE   GDataPathWidth;      //   Data path width: G/Y/H   1   Width of the data path for G/Y/H component in bits
   BYTE   BDataPathWidth;      //   Data path width: B/U/S/Pb/Cb   1   Width of the data path for B/U/S/Pb/Cb component in bits
   BYTE   RDataPathWidth;      //   Data path width: R/V/L/Pr/Cr   1   Width of the data path for R/V/L/Pr/Cr component in bits
}GammaCapabilitiesType;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct ColorSpaceCapabilitiesStruct
{
   GammaSupportType   SupportLevel;      //   Support level   1
   //0 - gamma compensation not supported
   //1 - supported as a separate table
   //2 - supported as a common table
   DWORD   GammaTableEntries;   //   Number of gamma table entries   4   Number of elements in the gamma table
   BYTE   GammaTableWidth;   //   Gamma table entry size   1   Width of a gamma table entry in bits
   DWORD   EntryOffset;      //   Entry offset   4   Gamma table entry offset. When the number of entries in the gamma table is less than the number of colors that can be displayed, some rounding will have to occur in the gamma block. The offset of 0 indicates that all values between two entries will be rounded down. The offset of N indicates that N points below the current point in the gamma table will be rounded up.
//BYTE   Segments;         //   Number of Segments   1   Number of segments for calculating the gamma table entries // Not used ...
}ColorSpaceCapabilitiesType;
#pragma pack(pop)


#pragma pack(push)
#pragma pack(1)
typedef struct
{
   WORD Shift:16;
   WORD DataStored:8;
   WORD CurrentCommand:5;
   WORD ColorCmpnt:3;
   WORD AllTables:1;
   WORD CalibrationOn:1;
   WORD StoredFlag:1;
   WORD UnCompletedElement:1;
   WORD ExtraData:1;
   WORD FirstMessage:1;
}GammaContextType;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct GammaGetSupportTablesStruct
{
   DWORD                    SupportTablesCapabilities;
}GammaGetSupportTablesType;
#pragma pack(pop)

//gamma v.2
#pragma pack(push)
#pragma pack(1)
typedef struct SMTGetCapabilities_Req
{
   BYTE                    Command;
   BYTE                    Color_Space;
   BYTE                    Color_component;
}gmt_SMTGetCapabilities_Req;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct SMTColor_Space_Req
{
   BYTE Command;
   BYTE Color_Space;
}gmt_SMTColor_Space_Req;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct SMTGetColorGamutMatrixCSC_Res
{
   BYTE Support_level;
   BYTE Table_entry_size;
	#if (GAMMA_SMT_PROTOCOL_VER >= VERSION_6)
	BYTE    Table_FractionalSize;   //in bits
	BYTE    Table_IntegralSize; //in bits
	#endif   
}gmt_SMTGetColorGamutMatrixCSC_Res;
#pragma pack(pop)

//===========================================================//gamma v.2

#pragma pack(push)
#pragma pack(1)
typedef struct GammaDownloadColorMappingGamutMatrix_s
{
   BYTE Status;
   WORD Coeff11;
   WORD Coeff12;
   WORD Coeff13;
   WORD Coeff21;
   WORD Coeff22;
   WORD Coeff23;
   WORD Coeff31;
   WORD Coeff32;
   WORD Coeff33;
   WORD Offset1;
   WORD Offset2;
   WORD Offset3;
}GammaDownloadColorMappingGamutMatrix_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct GammaUploadColorMappingGamutMatrix_s
{
   BYTE Command;
   BYTE ColorSpace;
   BYTE Channel;
   WORD Coeff11;
   WORD Coeff12;
   WORD Coeff13;
   WORD Coeff21;
   WORD Coeff22;
   WORD Coeff23;
   WORD Coeff31;
   WORD Coeff32;
   WORD Coeff33;
   WORD Offset1;
   WORD Offset2;
   WORD Offset3;
}GammaUploadColorMappingGamutMatrix_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct GammaUploadCompressedTable_Req
{
   BYTE Command;
   BYTE ColorSpace;
   BYTE ColorComponent;
   BYTE GammaTable;
   BYTE Channel;
}gmt_GammaUploadCoompressedTable_Req;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct SMTGetSteepTableColorspaceCapabilities_Res
{
   BYTE SupportLevel;
   DWORD NumberTableEntries;
   BYTE TableEntrySize;
   DWORD EntryOffset;
   #if 1
   BYTE SteepFinalEntrySize; // Width of the steep table final entry in bits
   WORD SteepTableSizeFactor;
   WORD SteepTabelAddressFactor;   
   #endif
}gmt_SMTGetSteepTableColorspaceCapabilities_Res;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct GammaDownloadLUTAndSteepTable_Req
{
   BYTE Command;
   BYTE ColorSpace;
   BYTE ColorComponent;
   BYTE GammaTable;
   BYTE LUTCompression;
   BYTE SteepCompression;
   BYTE Channel;
}gmt_GammaDownloadLUTAndSteepTable_Req;
#pragma pack(pop)

/**************************************************************************
 Definition for SMT transactions handler to determine context buffersize to be big enough for this prot handler
**************************************************************************/
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE sizeof(GammaContextType)
#elif SMT_MAX_BUF_SIZE < sizeof(GammaContextType)
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE sizeof(GammaContextType)
#endif
/**************************************************************************/


#define NUMBER_OF_SHADES ((FEATURE_VLUT_SIZE == 10)? 4096 : 1024)
#define NUMBER_OF_ELEMENTS_IN_GAMMA_LUT 1024 // ((FEATURE_VLUT_SIZE == 10) ? 1024 : 256)
#define NUMBER_OF_ELEMENTS_IN_GAMMA_STEEP_LUT 129 // 128 DisplayEditor Required 129 ...
#define GAMMA_TABLE_WIDTH 14 // ((FEATURE_VLUT_SIZE == 10) ? 12 : 10)
#define GAMMA_STEEP_TABLE_WIDTH 12 // ((FEATURE_VLUT_SIZE == 10) ? 12 : 10)
//#define DATA_WIDTH ((FEATURE_VLUT_SIZE == 10) ? 10 : 8)
//#define GAMMA_LUT_PADDING ((FEATURE_VLUT_SIZE == 10) ? 0x3f : 0xf)



//******************************************************************************
//  G L O B A L    F U N C T I O N
//******************************************************************************

BYTE GammaCorr_MsgHandler(MsgPacket_t *message, BYTE comm_use);
#endif
#endif

