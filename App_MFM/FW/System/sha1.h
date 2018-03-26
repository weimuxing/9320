//=============================================================================
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
//=============================================================================
//
//   MODULE :
//
//   USAGE  :
//
//   OWN    :
//
//=============================================================================
#ifndef __SHA1_H__
#define __SHA1_H__

#define SHA1HashSize 20
#define RX_HDCP_SHA_MIN_CHUNK_SIZE  64

#define SIZE_IN_WORDS(X) ((X) / sizeof(WORD))
#define SIZE_INTERNAL_VARS_BYTE (sizeof(DWORD) * 5)
#define SIZE_INTERNAL_VARS_WORD SIZE_IN_WORDS(sizeof(DWORD) * 5)
#define SIZE_DWORD_WORD SIZE_IN_WORDS(sizeof(DWORD))

typedef struct SHA1Context
{
   WORD Intermediate_Hash[SIZE_IN_WORDS(sizeof(DWORD) * 5)];//Message Digest
   BYTE wLen;//buffer length
   WORD pW_Msg[SIZE_IN_WORDS(512 / 8)];//temp buffer
   WORD wTotalLen;//total length
}gmt_SHA1_CONTEXT;

typedef struct
{
   gmt_SHA1_CONTEXT sha;
   BYTE Message_Digest[SHA1HashSize];
   BYTE Ba_InputData[RX_HDCP_SHA_MIN_CHUNK_SIZE]; //64 bytes
   BYTE B_DataIndex;
} gmt_SHA_STRUCT;

void far gm_SHA1Reset(gmt_SHA1_CONTEXT *);
void far gm_SHA1Input(gmt_SHA1_CONTEXT *, BYTE far *, WORD);
void far gm_SHA1Result(gmt_SHA1_CONTEXT *, BYTE Message_Digest[SHA1HashSize]);

#endif //__SHA1_H__ 

//********************************* END ****************************************
