#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
//#include "CommLink\SMT_AFM.h"

#ifdef USE_SMT


#define TS_AFM_DEBUG_MSG

#if defined(TS_AFM_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif


#define TS_AFM_VER 0
#define DYN_DAT_MAX_LEN 512


typedef enum
{
   TS_AFM_START,
   TS_AFM_STOP,
   TS_AFM_READ,
   TS_AFM_WRITE,
   TS_AFM_READ_DYNAMIC,
   TS_AFM_EN,
   TS_AFM_DISABLE
}StAfmCmd_t;

//#pragma pack(push)
//#pragma pack(1)
typedef struct
{
   WORD R;
   WORD S;
   WORD VM:8;
   WORD Phase:3;
   WORD IsFilm32:1; /* film mode 0-2:2, 1-3:2 */
   WORD IsFilmMode:1; /* 0-videomode, 1-filmmode */
}AfmDynDat_t;
//#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   DWORD FrameNum;
   DWORD Len ;//__attribute__((packed));
   //AfmDynDat_t AfmDat[0];// __attribute__((packed));//up to 512 entries table can be recided here
   AfmDynDat_t* AfmDat;
}TsAfmDyn_t;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct
{
   WORD TnrBin[8];
   WORD MIvpStat;
   DWORD CYWr;
   DWORD P1YRd;
   DWORD P2YRd;
   DWORD D1YRd;
   DWORD D2YRd;
   DWORD D1MvRd;
   DWORD D2MvRd;
   DWORD D1UVRd;
   DWORD D2UVRd;
   DWORD CUVWr;
   DWORD CUVRawWr;
   DWORD P2UVRawRd;
   DWORD DflPerErr;
   DWORD R;
   DWORD S;
   WORD Rcomp;
   WORD Scomp;
   BYTE Luma;
   BYTE CcsBin; /* 0-low, 1- std, 2-high */
   BYTE MadiBin; /* 0-low, 1- std, 2-high, 3-pan */
   BYTE TnrBinN; /* 0-low, 1- std, 2-high, 3-pan */
   BYTE TnrNoise;
   SBYTE TnrBias;
   WORD MadiNoise;
   BYTE CCSQuant0;
   BYTE CCSQuant1;
   BYTE MadiNoiseExt;
   BYTE MadiQuant0;
   BYTE MadiQuant1;
   BYTE MadiQuant2;
} AfmLogDebug_t;
#pragma pack(pop)

typedef enum
{
   STGXOBJ_PROGRESSIVE_SCAN,
   STGXOBJ_INTERLACED_SCAN
} STGXOBJ_ScanType_t;

typedef struct
{
#ifdef STORE_AFM_DEBUG_INFO
   WORD iReadDebug;
   AfmLogDebug_t LogDebug[DYN_DAT_MAX_LEN];
#endif
   BOOL Paused;
   BOOL isInterlaced;
   WORD iReadSmt;
   WORD iWrite;
   WORD iLast;
   DWORD luma;
   STGXOBJ_ScanType_t ScanType;
   AfmDynDat_t AfmData[DYN_DAT_MAX_LEN];
} AfmDataLogState_t;


// Enter 3:2
extern WORD gW_AlgVtToAfpThresh1;
extern WORD	gW_AlgVtToAfpMidlowThresh;

// Leave 3:2
extern WORD gW_AlgAfpToVtThresh2;
extern WORD	gW_AlgAfpToVtThresh3;
extern WORD	gW_AlgAfpToVtThresh4;
extern WORD	gW_AlgAfpToVtThresh5;


extern WORD gW_AlgVtToGfxMatches;
extern WORD gW_AlgGfxToVtThresh1;
extern WORD	gW_AlgGfxToVtThresh2;

extern WORD gW_AlgSwitch;
extern WORD gW_AlgAfpPersistThresh;
extern WORD gW_AlgAfpPersistThresh2;

extern WORD gDw_AlgGfxMaxLowS;
extern WORD gW_DetectionMode;

extern DWORD	gDw_AlgAfpPhase0MaxS;
extern DWORD	gDw_AlgMinSceneChange;

#if 0 // 1
//
// Algorithum threshold
//
WORD	gW_AlgVtToAfpThresh1 	  = 0x0028;		// 40/20=2; Rel thresh-avg R hi/lo for 3-2/2-2 entry
WORD	gW_AlgVtToGfxThresh1 	  = 0x008C;		// 140/20=7; Rel thresh-avg S hi/lo for 2-2 entry
WORD	gW_AlgAfpToVtThresh2 	  = 0x001E;		// 30/20=1.5;Rel thresh-float R hi/lo for 3-2 exit
WORD	gW_AlgAfpToVtThresh3 	  = 0x0016;		// 22/20=1.1;Rel thresh-avg R hi/lo for 3-2 exit
WORD	gW_AlgAfpToVtThresh4 	  = 0x0016;		// 22/20=1.1;Rel thresh-S hi/lo for 3-2 exit
WORD	gW_AlgAfpToVtThresh5 	  = 0x001E;		// 30/20=1.5;Rel thresh-float S hi/lo for 3-2 exit
WORD	gW_AlgGfxToVtThresh1 	  = 0x0064;		// 100/20=5;Rel thresh-S hi/lo for 2-2 exit
WORD	gW_AlgGfxToVtThresh2 	  = 0x001E;		// 30/20=1.5;Rel thresh-float S hi/lo for 2-2 exit
WORD	gW_AlgAfpPersistThresh 	  = 0x0019;		// 25/20=1.25;Rel thresh-3-2 persist
WORD	gW_AlgVtToAfpMidlowThresh =	0x0032;		// 50/20=2.5;Rel thresh-Alt S hi/lo for 3-2 entry
//
// Algorithm switches in ALG_SWITCH register
//
WORD	gW_AlgVtToAfpMatches 	= 0x0003;		// 3-2 entry latency (x5 fields)
WORD	gW_AlgVtToGfxMatches 	= 0x000F;		// 2-2 entry latency (fields)
WORD	gW_AlgSwitch			= 0x0007; 		// toggle various sub-algorithms
WORD	gW_AlgAfpPersistNm		= 0x0002; 		// consecutive persist matches for 3-2 exit
//
// Algorithm current swicthing mode
//
WORD	gW_CurrentState			= 0x0000;		//Current processing mode; 0=MADI, 4=3-2, 5=2-2
#endif

static volatile AfmDataLogState_t SMT_RAM LogAfm;


/*******************************************************************************
Description : Copies and "unwraps" RS Buffer circular data to specified address
Parameters  : LayerHandle, AfmDynDat_p
Assumptions : This function assumes the buffer and pointers are initialized
Limitations :
Returns     : U32 - number of rows copied
*******************************************************************************/
DWORD VCD_AfmLogRead(AfmDynDat_t* AfmDynDat_p, DWORD MaxSize)
{
   //if (LLD_IsAFMAvailable(Channel) && AfmDynDat_p)
   {
      DWORD Res = 0;

      LogAfm.Paused = TRUE; /* pause any logging activity */

      while ((LogAfm.iReadSmt > LogAfm.iWrite) && (MaxSize > 0))
      {   /* if buffer wrapped-around */
         memcpy(AfmDynDat_p++, (void *)&LogAfm.AfmData[LogAfm.iReadSmt], sizeof(AfmDynDat_t));
         LogAfm.iReadSmt = (LogAfm.iReadSmt + 1) % DYN_DAT_MAX_LEN;
         Res++;
         MaxSize--;
      }

      while ((LogAfm.iReadSmt < LogAfm.iWrite) && (MaxSize > 0))
      {   /* remained part */
         memcpy(AfmDynDat_p++, (void *)&LogAfm.AfmData[LogAfm.iReadSmt], sizeof(AfmDynDat_t));
         LogAfm.iReadSmt = (LogAfm.iReadSmt + 1) % DYN_DAT_MAX_LEN;
         Res++;
         MaxSize--;
      }

      LogAfm.Paused = FALSE; /* unpause logging activity */
      return Res;
   }
   //return 0;
}


#pragma argsused
BYTE AFM_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE* Bp_msg = (BYTE *)(message->msgpkt);

   //get pointer to output message length setting
   BYTE* Bp_len = GetOutMsgLenPtr();
   BYTE* Bp_out = GetOutMsgBufPtr();
   WORD AfmHeight, AfmVStart, AfmWidth, AfmHStart;

   AfmWidth = (stInput[CH_A].Hactive > gmvw_OutputHActive[CH_A]) ? gmvw_OutputHActive[CH_A]:stInput[CH_A].Hactive;

   AfmHeight = (stInput[CH_A].Vactive > gmvw_OutputVActive[CH_A]) ? gmvw_OutputVActive[CH_A] : stInput[CH_A].Vactive;

   if (Bp_msg[0] == TS_AFM_START)
   {
      Bp_out[0] = TS_OK;
      Bp_out[1] = TS_AFM_VER;
      *Bp_len = 2;
      dbgmsg("TS_AFM_START \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_STOP)
   {
      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_AFM_STOP \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_READ)
   {
      Bp_out[0] = TS_OK;

      // Size
      Bp_out[1] = 35; // ?
      Bp_out[2] = 0; // ?
      Bp_out[3] = 0; // ?
      Bp_out[4] = 0; // ?

      // Signature
      Bp_out[5] = 16; // ?
      Bp_out[6] = 0; // ?
      Bp_out[7] = 0; // ?
      Bp_out[8] = 0; // ?

      // Version ID
      Bp_out[9] = 0; // ?
      Bp_out[10] = 0; // ?
      Bp_out[11] = 0; // ?
      Bp_out[12] = 0; // ?

      // 0-enable all, 2-disable 3:2, 3-disable 2:2, 5-force MADI, 6-force 3:2, 8-force 2:2
      Bp_out[13] = gW_DetectionMode;

      // R
      Bp_out[14] = (BYTE)(gm_ReadRegWord(AFM_THRESH_RS)>>8);

      // S
      Bp_out[15] = gm_ReadRegByte(AFM_THRESH_RS);


      // VMThresh1
      Bp_out[16] = gm_ReadRegByte(AFM_22_CTRL1) & AFM_22_THRESH1;

      // VMThresh2
      Bp_out[17] = (BYTE)((gm_ReadRegWord(AFM_22_CTRL1) & AFM_22_THRESH2) >> 6);

      // VMThresh3
      Bp_out[18] = (BYTE)gm_ReadRegWord(AFM_22_CTRL2);

      // EnableHWThNoiseAdaptive // ? // default 0
      // Bp_out[19]

      // ThEnter32
      //const U8 row1[5] = { 1.1*20, 1.5*20, 2*20, 3*20, 4*20 };
      // gW_AlgVtToAfpThresh1;  // default is 40 should return 2 ?
      if (gW_AlgVtToAfpThresh1 == 22)
         Bp_out[20] = 0;
      else if (gW_AlgVtToAfpThresh1 == 30)
         Bp_out[20] = 1;
      else if (gW_AlgVtToAfpThresh1 == 40)
         Bp_out[20] = 2;
      else if (gW_AlgVtToAfpThresh1 == 60)
         Bp_out[20] = 3;
      else
         Bp_out[20] = 4;

      // ThLeave32
      // const U8 row2[5] = { 3*20, 2*20, 1.5*20, 1.2*20, 1*20 };
      // gW_AlgAfpToVtThresh2 default is 30 should return 2
      if (gW_AlgAfpToVtThresh2 == 80)
         Bp_out[21] = 0;
      else if (gW_AlgAfpToVtThresh2 == 60)
         Bp_out[21] = 1;
      else if (gW_AlgAfpToVtThresh2 == 40)
         Bp_out[21] = 2;
      else if (gW_AlgAfpToVtThresh2 == 30)
         Bp_out[21] = 3;
      else
         Bp_out[21] = 4;

      // ThEnter22
      // const U16 row1[5] = { 3, 8, 15, 20, 30 };
      // gW_AlgVtToGfxMatches defualt is 15 should return 2
      if (gW_AlgVtToGfxMatches == 3)
         Bp_out[22] = 0;
      else if (gW_AlgVtToGfxMatches == 8)
         Bp_out[22] = 1;
      else if (gW_AlgVtToGfxMatches == 15)
         Bp_out[22] = 2;
      else if (gW_AlgVtToGfxMatches == 20)
         Bp_out[22] = 3;
      else
         Bp_out[22] = 4;

      // ThLeave22
      // const U8 row1[5] = { 7*20, 6*20, 5*20, 3*20, 1*20 };
      // gW_AlgGfxToVtThresh1 default is 100 should return 2
      if (gW_AlgGfxToVtThresh1 == 140)
         Bp_out[23] = 0;
      else if (gW_AlgGfxToVtThresh1 == 120)
         Bp_out[23] = 1;
      else if (gW_AlgGfxToVtThresh1 == 100)
         Bp_out[23] = 2;
      else if (gW_AlgGfxToVtThresh1 == 60)
         Bp_out[23] = 3;
      else
         Bp_out[23] = 4;

      // Enable32Persis
      // switch BIT0
      // gW_AlgSwitch
      Bp_out[24] = (BYTE)(gW_AlgSwitch & (~BIT0));

      // AFM_32Persis
      // const U16 row1[5] = { 1.05*20, 1.1*20, 1.25*20, 1.5*20, 3*20 };
      // gW_AlgAfpPersistThresh defualt is 25 should return 2
      if (gW_AlgAfpPersistThresh == 21)
         Bp_out[25] = 0;
      else if (gW_AlgAfpPersistThresh == 22)
         Bp_out[25] = 1;
      else if (gW_AlgAfpPersistThresh == 25)
         Bp_out[25] = 2;
      else if (gW_AlgAfpPersistThresh == 30)
         Bp_out[25] = 3;
      else
         Bp_out[25] = 4;

      // Enable22Persis
      // switch BIT1
      // gW_AlgSwitch
      Bp_out[26] = (BYTE)(gW_AlgSwitch & (~BIT1)) >> 1;

      // AFM_22Persis
      // const U16 row1[5] = { 256, 2047, 4296, 32767, 65535 };
      // gDw_AlgGfxMaxLowS
      if (gDw_AlgGfxMaxLowS == 256)
         Bp_out[27] = 0;
      else if (gDw_AlgGfxMaxLowS == 2047)
         Bp_out[27] = 1;
      else if (gDw_AlgGfxMaxLowS == 4296)
         Bp_out[27] = 2;
      else if (gDw_AlgGfxMaxLowS == 32767)
         Bp_out[27] = 3;
      else
         Bp_out[27] = 4;

      // LowSignalThreshold
		Bp_out[28] = (BYTE)gDw_AlgAfpPhase0MaxS; // Need to check the align

      // SceneChangeThreshold
		Bp_out[29] = (BYTE)gDw_AlgMinSceneChange; // Need to check the align

      // Rej32ShortSeq
      // switch BIT2
      // gW_AlgSwitch
      Bp_out[30] = (BYTE)(gW_AlgSwitch & (~BIT2)) >> 2;

      // Rej32LowMotion
      // switch BIT4
      // gW_AlgSwitch
      Bp_out[31] = (BYTE)(gW_AlgSwitch & (~BIT4)) >> 4;



      // WinLeft
      Bp_out[32] = (BYTE)((DWORD)gm_ReadRegWord(AFM_HSTART) * 1000 / AfmWidth);

      // WinRight
      Bp_out[33] = (BYTE)((DWORD)(AfmWidth - gm_ReadRegWord(AFM_HCOUNT) - gm_ReadRegByte(AFM_HSTART)) * 1000 / AfmWidth);

      // WinTop
      Bp_out[34] = (BYTE)((DWORD)gm_ReadRegWord(AFM_VSTART) * 1000 / AfmHeight);

      // WinBottom
      Bp_out[35] = (BYTE)((DWORD)(AfmHeight - gm_ReadRegWord(AFM_VCOUNT) - gm_ReadRegByte(AFM_VSTART)) * 1000 / AfmHeight);

      *Bp_len = 36;
      dbgmsg("TS_AFM_READ \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_WRITE)
   {
      // AfmMode
      gW_DetectionMode = Bp_msg[14];

      // RTh // STh
      gm_WriteRegWord(AFM_THRESH_RS, (((WORD)Bp_msg[15]<<8)|Bp_msg[16]));

      // VMTh1 // VMTh2
      gm_WriteRegWord(AFM_22_CTRL1, (((WORD)Bp_msg[18]<<6) | Bp_msg[17]));

      // VMTh3
      gm_WriteRegWord(AFM_22_CTRL2, (WORD)Bp_msg[19]);

      // EnableHWThNoiseAdaptive
      // Bp_msg[20] // ?

      // ThEnter32
      // const U8 row1[5] = { 1.1*20, 1.5*20, 2*20, 3*20, 4*20 };
      // const U8 row2[5] = { 1.1*20, 1.8*20, 2.5*20, 5*20, 7*20 };
      if (Bp_msg[21] == 0)
      {
         gW_AlgVtToAfpThresh1 = 22;
         gW_AlgVtToAfpMidlowThresh = 22;
      }
      else if (Bp_msg[21] == 1)
      {
         gW_AlgVtToAfpThresh1 = 30;
         gW_AlgVtToAfpMidlowThresh = 36;
      }
      else if (Bp_msg[21] == 2)
      {
         gW_AlgVtToAfpThresh1 = 40;
         gW_AlgVtToAfpMidlowThresh = 50;
      }
      else if (Bp_msg[21] == 3)
      {
         gW_AlgVtToAfpThresh1 = 60;
         gW_AlgVtToAfpMidlowThresh = 100;
      }
      else
      {
         gW_AlgVtToAfpThresh1 = 80;
         gW_AlgVtToAfpMidlowThresh = 140;
      }

      // ThLeave32
      // const U8 row1[5] = { 4*20, 3*20, 2*20, 1.5*20, 1*20 };
      // const U8 row2[5] = { 3*20, 2*20, 1.5*20, 1.2*20, 1*20 };
      // const U8 row3[5] = { 1.2*20, 1.15*20, 1.1*20, 1.05*20, 1*20 };
      // const U8 row4[5] = { 3*20, 2*20, 1.5*20, 1.25*20, 1*20 };
      if (Bp_msg[22] == 0)
      {
         gW_AlgAfpToVtThresh2 = 80;
         gW_AlgAfpToVtThresh3 = 60;
         gW_AlgAfpToVtThresh4 = 24;
         gW_AlgAfpToVtThresh5 = 60;
      }
      else if (Bp_msg[22] == 1)
      {
         gW_AlgAfpToVtThresh2 = 60;
         gW_AlgAfpToVtThresh3 = 40;
         gW_AlgAfpToVtThresh4 = 23;
         gW_AlgAfpToVtThresh5 = 40;
      }
      else if (Bp_msg[22] == 2)
      {
         gW_AlgAfpToVtThresh2 = 40;
         gW_AlgAfpToVtThresh3 = 30;
         gW_AlgAfpToVtThresh4 = 22;
         gW_AlgAfpToVtThresh5 = 30;
      }
      else if (Bp_msg[22] == 3)
      {
         gW_AlgAfpToVtThresh2 = 30;
         gW_AlgAfpToVtThresh3 = 24;
         gW_AlgAfpToVtThresh4 = 21;
         gW_AlgAfpToVtThresh5 = 25;
      }
      else
      {
         gW_AlgAfpToVtThresh2 = 20;
         gW_AlgAfpToVtThresh3 = 20;
         gW_AlgAfpToVtThresh4 = 20;
         gW_AlgAfpToVtThresh5 = 20;
      }

      // ThEnter22
      // const U16 row1[5] = { 3, 8, 15, 20, 30 };
      if (Bp_msg[23] == 0)
         gW_AlgVtToGfxMatches = 3;
      else if (Bp_msg[23] == 1)
         gW_AlgVtToGfxMatches = 8;
      else if (Bp_msg[23] == 2)
         gW_AlgVtToGfxMatches = 15;
      else if (Bp_msg[23] == 3)
         gW_AlgVtToGfxMatches = 20;
      else
         gW_AlgVtToGfxMatches = 30;

      // ThLeave22
      // const U8 row1[5] = { 140, 120, 100, 60, 20 };
      // const U8 row2[5] = { 60, 40, 30, 25, 20 };
      if (Bp_msg[24] == 0)
      {
         gW_AlgGfxToVtThresh1 = 140;
         gW_AlgGfxToVtThresh2 = 60;
      }
      else if (Bp_msg[24] == 1)
      {
         gW_AlgGfxToVtThresh1 = 120;
         gW_AlgGfxToVtThresh2 = 40;
      }
      else if (Bp_msg[24] == 2)
      {
         gW_AlgGfxToVtThresh1 = 100;
         gW_AlgGfxToVtThresh2 = 30;
      }
      else if (Bp_msg[24] == 3)
      {
         gW_AlgGfxToVtThresh1 = 60;
         gW_AlgGfxToVtThresh2 = 25;
      }
      else
      {
         gW_AlgGfxToVtThresh1 = 20;
         gW_AlgGfxToVtThresh2 = 20;
      }

      // Enable32Persis
      // switch BIT0
      // gW_AlgSwitch
      // Bp_out[25] = (BYTE)(gW_AlgSwitch & (~BIT0));

      // Enable22Persis
      // switch BIT1
      // gW_AlgSwitch
      // Bp_msg[27] = (BYTE)(gW_AlgSwitch & (~BIT1)) >> 1;

      // Rej32ShortSeq
      // switch BIT2
      // gW_AlgSwitch
      //Bp_msg[31] = (BYTE)(gW_AlgSwitch & (~BIT2)) >> 2;

      // Rej32LowMotion
      // switch BIT4
      // gW_AlgSwitch
      // Bp_msg[32] = (BYTE)(gW_AlgSwitch & (~BIT4)) >> 4;

      gW_AlgSwitch = (Bp_msg[32]<<4)|(Bp_msg[31]<<2)|(Bp_msg[27] << 1)|Bp_msg[25];

      // AFM_32Persis
      // const U16 row1[5] = { 21, 22, 25, 30, 60 };
      // const U16 row2[5] = { 21, 22, 25, 26, 30 };
      if (Bp_msg[26] == 0)
      {
         gW_AlgAfpPersistThresh = 21;
         gW_AlgAfpPersistThresh2 = 21;
      }
      else if (Bp_msg[26] == 1)
      {
         gW_AlgAfpPersistThresh = 22;
         gW_AlgAfpPersistThresh2 = 22;
      }
      else if (Bp_msg[26] == 2)
      {
         gW_AlgAfpPersistThresh = 25;
         gW_AlgAfpPersistThresh2 = 25;
      }
      else if (Bp_msg[26] == 3)
      {
         gW_AlgAfpPersistThresh = 30;
         gW_AlgAfpPersistThresh2 = 26;
      }
      else
      {
         gW_AlgAfpPersistThresh = 60;
         gW_AlgAfpPersistThresh2 = 30;
      }

      // AFM_22Persis
      // const U16 row1[5] = { 256, 2047, 4296, 32767, 65535 };
      // gDw_AlgGfxMaxLowS
      if (Bp_msg[28] == 0)
         gDw_AlgGfxMaxLowS = 256;
      else if (Bp_msg[28] == 1)
         gDw_AlgGfxMaxLowS = 2047;
      else if (Bp_msg[28] == 2)
         gDw_AlgGfxMaxLowS = 4296;
      else if (Bp_msg[28] == 3)
         gDw_AlgGfxMaxLowS = 32767;
      else
         gDw_AlgGfxMaxLowS = 0xFFFF;

      // LowSignalThreshold
      gDw_AlgAfpPhase0MaxS = Bp_msg[29];

      // SceneChangeThreshold
      gDw_AlgMinSceneChange = Bp_msg[30];

      // WinLeft
      // scale 100
      // Bp_msg[33] = (BYTE)(gm_ReadRegWord(AFM_HSTART) * 100 / gmvw_OutputHActive[CH_A]);
      gm_WriteRegWord(AFM_HSTART, (WORD)((DWORD)Bp_msg[33]*AfmWidth/1000));

      // WinRight
      // Bp_msg[34] = (BYTE)((gmvw_OutputHActive[CH_A] + gm_ReadRegByte(AFM_HSTART)) * 100 / gmvw_OutputHActive[CH_A]);
      // AfmWidth -= AfmHStart + (U16)((U32)AfmWidth * AFMData_p->WinRight / 100);
      AfmHStart = gm_ReadRegWord(AFM_HSTART);

      AfmWidth -= AfmHStart + (WORD)((DWORD)AfmWidth * Bp_msg[34] / 1000);
      gm_WriteRegWord(AFM_HCOUNT, AfmWidth);

      // WinTop
      // AfmVStart = (U16)((U32) AfmHeight * AFMData_p->WinTop / 100);
      gm_WriteRegWord(AFM_VSTART, (WORD)((DWORD)Bp_msg[35]*AfmHeight/1000));

      // WinBottom
      //Bp_msg[36] = (BYTE)((gmvw_OutputVActive[CH_A] + gm_ReadRegByte(AFM_VSTART)) * 100 / gmvw_OutputVActive[CH_A]);
      // AfmHeight -= AfmVStart + (U16)((U32) AfmHeight * AFMData_p->WinBottom / 100);
      AfmVStart = gm_ReadRegWord(AFM_VSTART);

      AfmHeight -= AfmVStart + (WORD)((DWORD) AfmHeight * Bp_msg[36] / 1000);

      gm_WriteRegWord(AFM_VCOUNT, AfmHeight);

      Bp_out[0] = TS_OK;
      *Bp_len = 1;
      dbgmsg("TS_AFM_WRITE \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_READ_DYNAMIC)
   {
      // FrameNum 4 bytes

      // Length 2 bytes

      // AfmDynamicData[] N*6 bytes
      // AFM_R 2  bytes
      // AFM_S 2 bytes
      // AFM_VM 1 bytes
      // AFM_CurState 1 bytes

      DWORD Len;
      static DWORD debug_framenum;
      ((TsAfmDyn_t *)(Bp_out+1))->Len = Len = VCD_AfmLogRead(((TsAfmDyn_t *)(Bp_out+1))->AfmDat, 512);
      debug_framenum += Len;
      ((TsAfmDyn_t *)(Bp_out+1))->FrameNum = debug_framenum;

      Bp_out[0] = TS_OK;

      if (Len)
         *Bp_len = 1 + sizeof(TsAfmDyn_t) + (BYTE)(Len * sizeof(AfmDynDat_t));
      else
         *Bp_len = 1 + 6; // ?

      dbgmsg("TS_AFM_READ_DYNAMIC \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_EN)
   {
      Bp_out[0] = TS_OK;

      gm_SetRegBitsWord(MOTION_PROC_CTRL, AFM_DETECT_EN);

      *Bp_len = 1;
      dbgmsg("TS_AFM_EN \n", 0);
   }
   else if (Bp_msg[0] == TS_AFM_DISABLE)
   {
      Bp_out[0] = TS_OK;

      gm_ClearRegBitsWord(MOTION_PROC_CTRL, AFM_DETECT_EN);

      *Bp_len = 1;
      dbgmsg("TS_AFM_DISABLE \n", 0);
   }
   else
   {
      dbgmsg("STTS_Handler_AFM: UNKNOWN COMMAND \n", 0);
      Bp_out[0] = TS_NOT_SUPPORTED;
      *Bp_len = 1;
   }


   return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif

