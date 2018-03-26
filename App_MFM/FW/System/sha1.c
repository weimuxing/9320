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
//   MODULE : sha1.c
//
//   USAGE  : This module contains functions to calculate hash function sha1
//            sha1 implementation taken from RFC3174
//            This file implements the Secure Hashing Algorithm 1 as
//            defined in FIPS PUB 180-1 published April 17, 1995.
//            The SHA-1, produces a 160-bit message digest for a given
//            data stream.  It should take about 2**n steps to find a
//            message with the same digest as a given message and
//            2**(n/2) to find any two messages with the same digest,
//            when n is the digest size in bits.  Therefore, this
//            algorithm can serve as a means of providing a
//            "fingerprint" for a message.
//
//   OWN    :
//
//=============================================================================


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include <mem.h>

#if (FEATURE_DISPLAY_PORT == ENABLE && defined(CFG_DPTX_HDCP_EN)) || (FEATURE_HDMI == ENABLE)

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define A (pInternalVars    )
#define B (pInternalVars + 2)
#define C (pInternalVars + 4)
#define D (pInternalVars + 6)
#define E (pInternalVars + 8)


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

static ROM DWORD pK_Words[] =
{
   0x5A827999UL,
   0x6ED9EBA1UL,
   0x8F1BBCDCUL,
   0xCA62C1D6UL
};

static ROM DWORD IH_0[5] =
{
   0x67452301UL,
   0xEFCDAB89UL,
   0x98BADCFEUL,
   0x10325476UL,
   0xC3D2E1F0UL
};

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void SHA1_Kernel(WORD * pH, WORD * pW);
static void near SHA1_KernelWrapper(WORD * pH, BYTE * pInput);
static void gmi_SHA1_Pad(gmt_SHA1_CONTEXT * pCntx);

static void gmi_SwapDWordInArray(BYTE *pDst, BYTE *pSrc, WORD nLenInDwords);
//******************************************************************************
//  C O D E
//******************************************************************************

//*****************************************************************************
// FUNCTION     : static void SHA1_Kernel(WORD * pH, WORD * pW)
// USAGE        : Kernel of SHA-1 algorithm
// INPUT        : H and W
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
static void SHA1_Kernel(WORD * pH, WORD * pW)
{
   BYTE i, t;
   WORD pInternalVars[SIZE_INTERNAL_VARS_WORD];//a, b, c, d, e
   WORD pF[SIZE_DWORD_WORD], pTmp[SIZE_DWORD_WORD], pTmp2[SIZE_DWORD_WORD];
   _fmemcpy(pInternalVars, pH, SIZE_INTERNAL_VARS_BYTE);
   for (i = 0; i < 80; i++)
   {
      t = i / 20;
      if (t == 0)
      {
         pTmp[0] = (~B[0]) & D[0];
         pTmp[1] = (~B[1]) & D[1];
         pF[0] = B[0] & C[0];
         pF[1] = B[1] & C[1];
         pF[0] |= pTmp[0];
         pF[1] |= pTmp[1];
      }
      else if (t == 1)
      {
         pF[0] = B[0] ^ C[0] ^ D[0];
         pF[1] = B[1] ^ C[1] ^ D[1];
      }
      else if (t == 2)
      {
         pF[0] = B[0] & C[0];
         pF[1] = B[1] & C[1];
         pTmp[0] = B[0] & D[0];
         pTmp[1] = B[1] & D[1];
         pF[0] |= pTmp[0];
         pF[1] |= pTmp[1];
         pTmp[0] = C[0] & D[0];
         pTmp[1] = C[1] & D[1];
         pF[0] |= pTmp[0];
         pF[1] |= pTmp[1];
      }
      else
      {
         pF[0] = B[0] ^ C[0] ^ D[0];
         pF[1] = B[1] ^ C[1] ^ D[1];
      }

      //lefrotate 5
      pTmp[0] = A[0];
      pTmp[1] = A[1];
      pTmp2[0] = (pTmp[0] << 5) | (pTmp[1] >> 11);
      pTmp2[1] = (pTmp[1] << 5) | (pTmp[0] >> 11);

      *(DWORD *)pTmp = *(DWORD *)pTmp2 + *(DWORD *)pF + *(DWORD *)E + pK_Words[t] + *(DWORD *)(pW + (i << 1));

      //
      _fmemmove(B, A, sizeof(DWORD) * 4);
      A[0] = pTmp[0];
      A[1] = pTmp[1];
      //
      //lefrotate 30
      pTmp[0] = C[0];
      pTmp[1] = C[1];
      C[0] = (pTmp[0] >> 2)  | (pTmp[1] << 14);
      C[1] = (pTmp[0] << 14) | (pTmp[1] >> 2);

   }
   for (i = 0; i < 5; i++) ((DWORD *)pH)[i] += ((DWORD *)pInternalVars)[i];
}

//*****************************************************************************
// FUNCTION     : static void SHA1_KernelWrapper(WORD * pH, BYTE * pInput)
// USAGE        : wrapper for kernel of SHA-1 algorithm
// INPUT        : H and Input
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
static void near SHA1_KernelWrapper(WORD * pH, BYTE * pInput)
{
   WORD pW[SIZE_IN_WORDS(sizeof(DWORD) * 80)];
   BYTE i;
   WORD pTmp[2];
   BYTE * pW_B = (BYTE *)pW;

   gmi_SwapDWordInArray(pW_B, pInput, 16);

   for (i = 16; i < 80; i++)
   {
      {
         WORD * pW_Tmp = &pW[i << 1];
         WORD * pW_Res = pW_Tmp;

         pTmp[0] = pW_Tmp[0] = pW_Tmp[-6] ^ pW_Tmp[-16] ^ pW_Tmp[-28] ^ pW_Tmp[-32];
         //pTmp[0] = *pW_Tmp;
         pW_Tmp++;

         pTmp[1] = pW_Tmp[0] = pW_Tmp[-6] ^ pW_Tmp[-16] ^ pW_Tmp[-28] ^ pW_Tmp[-32];
         //pTmp[1] = *pW_Tmp;
         pW_Res[0] <<= 1;
         if (pTmp[1] & 0x8000) pW_Res[0] |= 1;
         pW_Res++;
         pW_Res[0] <<= 1;
         if (pTmp[0] & 0x8000) pW_Res[0] |= 1;
      }
   }
   SHA1_Kernel(pH, pW);
}

//*****************************************************************************
// FUNCTION     : void far gmi_SHA1Reset(gmt_SHA1_CONTEXT * pCntx)
// USAGE        : reset SHA-1 context
// INPUT        : pointer to context
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
void far gm_SHA1Reset(gmt_SHA1_CONTEXT * pCntx)
{
   _fmemcpy(pCntx->Intermediate_Hash, IH_0, sizeof(DWORD) * 5);
   pCntx->wLen = 0;
   pCntx->wTotalLen = 0;
}

//*****************************************************************************
// FUNCTION     : void far gmi_SHA1Input(gmt_SHA1_CONTEXT * pCntx, const BYTE * pIn, WORD wLen)
// USAGE        : append data to SHA-1 context and calcutate hash
// INPUT        : pointer to context, pointer to data and data length
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
void far gm_SHA1Input(gmt_SHA1_CONTEXT * pCntx, BYTE far * pIn, WORD wLen)
{
   BYTE wCnt = pCntx->wLen;
   BYTE * pBt = (BYTE *)pCntx->pW_Msg;
   pBt += wCnt;
   pCntx->wTotalLen += wLen;
   while (wLen)
   {
      *pBt++ = *pIn++;
      wCnt++;
      if (wCnt == 64)
      {
         pBt = (BYTE *)pCntx->pW_Msg;
         SHA1_KernelWrapper(pCntx->Intermediate_Hash, pBt);
         wCnt = 0;
      }
      wLen--;
   }
   pCntx->wLen = wCnt;
}

//*****************************************************************************
// FUNCTION     : void far gmi_SHA1Result(gmt_SHA1_CONTEXT * pCntx, BYTE Message_Digest[SHA1HashSize])
// USAGE        : returns SHA-1 hash
// INPUT        : pointer to context, pointer to result (hash)
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
void far gm_SHA1Result(gmt_SHA1_CONTEXT * pCntx, BYTE Message_Digest[SHA1HashSize])
{
   gmi_SHA1_Pad(pCntx);
   gmi_SwapDWordInArray((BYTE *)Message_Digest, (BYTE *)pCntx->Intermediate_Hash, SHA1HashSize/4);
}


//*****************************************************************************
// FUNCTION     : static void gmi_SHA1_Pad(gmt_SHA1_CONTEXT * pCntx)
// USAGE        : add padding bytes
// INPUT        : pointer to context
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
static void gmi_SHA1_Pad(gmt_SHA1_CONTEXT * pCntx)
{

   BYTE * pBt =  (BYTE *)pCntx->pW_Msg;
   WORD wLen = pCntx->wLen;
   DWORD dwTot;
   BYTE * pTot = (BYTE *)&dwTot;
   pTot[0] = 0;
   pTot[3] = 0;
   *((WORD *)&(pTot[1])) = pCntx->wTotalLen;
   pBt += wLen;

   // Check to see if the current message block is too small to hold
   // the initial padding bits and length.  If so, we will pad the
   // block, process it, and then continue padding into a second
   // block.

   *pBt++ = 0x80;
   if ( wLen > 55 )
   {
      _fmemset(pBt, 0x00, 63 - wLen);
      SHA1_KernelWrapper(pCntx->Intermediate_Hash, (BYTE *)pCntx->pW_Msg);
      _fmemset(pCntx->pW_Msg, 0x00, 59);
   }
   else
   {
      _fmemset(pBt, 0x00, 58 - wLen);
   }

   pBt =  (BYTE *)pCntx->pW_Msg;
   pBt += 59;

   if (pTot[1] | pTot[2])
   {
      *pBt++ = 0x00;
   }
   else *pBt++ = 1;

   *((WORD *)&(pTot[0])) >>= 5;
   pTot[1] |= (pTot[2] & 31) << 3;
   *((WORD *)&(pTot[2])) >>= 5;

   gmi_SwapDWordInArray(pBt, pTot, 1);

   SHA1_KernelWrapper(pCntx->Intermediate_Hash, (BYTE *)pCntx->pW_Msg);

}

//*****************************************************************************
// FUNCTION     : static void gmi_SwapDWordInArray(BYTE *pDst, BYTE *pSrc, WORD nLenInDwords)
// USAGE        : swap bytes in dword array
// INPUT        : pointer to DST, SRC and length in dwords
// OUTPUT       : none
// GLOBALS      : none
// USED_REGS    : none
//                PRE_CONDITION  : none
//                POST_CONDITION : none
//*****************************************************************************
static void gmi_SwapDWordInArray(BYTE *pDst, BYTE *pSrc, WORD nLenInDwords)
{
   __asm
   {
      mov cx, nLenInDwords
      mov di, pDst
      mov si, pSrc
Swap_loop:
      mov al, byte ptr[si + 3];
      mov ah, byte ptr[si + 2];
      mov word ptr[di], ax;

      mov al, byte ptr[si + 1];
      mov ah, byte ptr[si + 0];
      mov word ptr[di + 2], ax;

      add si, 4
      add di, 4
      dec cx
      jnz Swap_loop;
   }
}


#endif
//********************************* END ****************************************
