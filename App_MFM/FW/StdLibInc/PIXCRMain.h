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
// MODULE:      PIXCRMain.h
//
// USAGE :      Header file that defines pixelCrunher .
//
//******************************************************************

#define REALHW

#ifndef REALHW
 #define ROM
 #define	far
#endif

//#define WINSTEPSIZE     4  // moved as code variable change from 3 to 4 based on Dither Y-Region size (4 lines when it's 2)
#define MINWINWIDTH     5
#define MINWINHEIGHT    5
#define NUMEDGEREGION   1
#define BLACKLEVEL      8 
#define FINEBLACKLEVEL  12
#define NUMBEROFRUNS_0  4
//#define NUMBEROFCAPTURES 18 //moved as code variable changes based on DitherDetect
#define FINENUMOFRUN    3
#define XREGNSPERRUN_0  8
#define NUMBEROFXRGNS_0 (NUMBEROFRUNS_0*XREGNSPERRUN_0)
#define NUMBEROFYRGNS_0 30
#define FINENUMOVERLAPREG 3

#define NUMBEROFBUCKETS_0 128
#define YREGIONGAP_0 1

#define VERTZOOM1TOPBOTT  8 	// how much to miss from top & bottom
#define VERTZOOM1NUMXRGNS 16	// how many X regions for vertical edge zoom
#define HOZOOMNUMYRGNS 16		// number of Y regions to use in horizontal zoom
#define HOZOOM1TOPBOTT 8// how much to miss from left & right
#define HOZOOMYSIZE 2		// Y region size in horizontal zoom mode
#define HOZOOMYGAP 1			// Y Region gap in horizontal zoom mode
#define HOZOOMLENGTH (HOZOOMNUMYRGNS*(HOZOOMYSIZE+HOZOOMYGAP))	// total length of area to be searched

#define EdgeThreshold 30
#define EDGEDIVFACT 20 
#define COARSEMOTIONTHRESHOLD 0 
#define MOTIONTHRESHOLD 0 
#define MOTDIFFPERPIXEL 255 
#define INTTHRESHOLD 200 
#define INTHISTTHRESHOLD 10
//#define NOOFFRAMEMOTKEPT 6 
#define REALALL 80
#define RATIOTHRESHOLD 150 
#define FINENUMMOTREG 0
#define FINENUMNOBLACKREG 4
#define RECTCORNERADJREG 4
#define BORDERNUMOFDIFFREG 2
#define BORDER_IN_SIDENUMDIFFREG 0
#define BORDER_IN_TOTALNUMDIFFREG 4
#define BORDER_IN_SUMSIDE 0
#define DITHERVALIDBLOCKMININT 64 
#define DITHERVALIDBLOCKMAXINT 254 
#define DITHERTOSTATICRATIO 64 
#define DITHERMINBLOCK 25 
#define DITHERVALIDBLOCKS 25
#define DITHERCORPERCENTAGE 2
#define DITHERCOARSEMULTFACT 1
#define DITHERFINEREDUCEFACT 2
#define DITHERDETECTMEASNUMCAP 11
#define COARSEFINE_SEPRGBMOT_ONDITHER 1
#define COARSEFINE_SEPRGBMOT_NODITHER 0
#define RGBIMPULSEFIX
//Three side video window enable using LASTEDGEFOUND_ON 2 , disable LASTEDGEFOUND_ON 0
#define LASTEDGEFOUND_ON 2
//#define DITHERMEAS_ENABLE 
//#define Int_EDGETHR_FLD 8 
#define Intl_EDGETHR 10 
#define Intl_EDGECorthr 6
#define Intl_EDGEloclowthr 6 
#define Intl_EDGElochighthr 10
#define Intl_CorThr 3
#define Intl_CorMotThr 0 
#define Intl_OverUnderThr 6 
#define Intl_doubleOU_Thr 6
#define Intl_FineMotionThr 1
#define Intl_EdgeSliceThr 1
#define Intl_C1thr 0
#define Intl_C2thr 1
#define Intl_AFSumthr 2
//Not used////////
#define Intl_MinEdgeThr 1
#define Intl_EdgeDivFactor 10 
/////////////////
#define FULLSedgecountThr 2
#define FULLSgoingInScore 25
#define FULLScomingOutScore 45
#define FULLSScoreCE 3
#define FULLSstablecountThr 1


#define PIXCR_RAM far

typedef enum
{
   ANALYZE_LUMA,
   ANALYZE_RED,
   ANALYZE_GREEN,
   ANALYZE_BLUE,
   ANALYZE_UPV,
   ANALYZE_ACC,
   ANALYZE_EDGE,
   ANALYZE_R_ACC,
   ANALYZE_G_ACC,
   ANALYZE_B_ACC,
}gmt_AnalyzeParam;

// the video types
typedef enum
{
   FULL_SCREEN     = 0,
   LETTER_BOX      = 1,
   PILLAR_BOX      = 2,
   SQUARE_IMAGE    = 3,
   UNKNOWN_IMAGE   = 4,
   LOST_TYPE       = 5,
   FULL_TEXT       = 6,
   NO_ARTIFACT     = 7,
   ARTIFACT        = 8
}gmt_VideoType;

typedef struct
{
   gmt_VideoType VideoType;      // 1 word = 2 byte
   WORD XStart;                  // 1 word = 2 byte
   WORD YStart;                  // 1 word = 2 byte
   WORD XEnd;                    // 1 word = 2 byte
   WORD YEnd;                    // 1 word = 2 byte
   BYTE COUNT;                   // 1 byte
}gmt_VideoInfo;                  // => 11 byte

typedef struct
{
   BYTE DEBUGONOFF;
   BYTE BLACKONOFF;
   BYTE BORDERLESSONOFF;
   BYTE COA_EDGEHWTHR; 
   BYTE COA_EDGEDIVFACT;
   WORD COA_MOTIONTHR;
   BYTE COA_MOTIONGATE;
   BYTE COA_REALTHR;
   BYTE COA_REALGATE;
   BYTE COA_BLACKTHR;
   BYTE COA_FINALGATEMOTION;
   BYTE COA_FINALGATE;
   BYTE COA_FINALMAPSEL; 
   BYTE COA_RECTWIDTH; 
   BYTE COA_RECTHEIGHT; 
   BYTE FIN_MOTIONTHR; 
   BYTE FIN_BLACKTHR; 
   BYTE FORCE_DITHER;
} gmt_PCInputConfig;

typedef struct
{
   gmt_VideoType VideoType;      // 1 word = 2 byte
   BYTE XStart;                  // 1 byte
   BYTE YStart;                  // 1 byte
   BYTE XEnd;                    // 1 byte
   BYTE YEnd;                    // 1 byte
   WORD Score;                   // 1 word = 2 byte
}gmt_VideoInfoByte;              // => 8 byte

typedef struct
{
	gmt_VideoInfo Rect[4];
}gmt_MultiVideoInfo;

typedef struct
{
  DWORD INTXY[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];            // 30*32 = 960 dword = 3840 byte
  //DWORD RpGpB[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];            // 30*32 = 960 dword = 3840 byte
  DWORD INT_RGB[3][NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];            // 30*32 = 960 dword = 3840 byte
  DWORD MotionRGB[6][NUMBEROFXRGNS_0];                         // 32 dword = 128 byte
  BYTE Motion[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];            // 30*32 = 960 byte
  DWORD Edgearray[NUMBEROFXRGNS_0];                         // 32 dword = 128 byte
  DWORD EdgearrayPrev[NUMBEROFXRGNS_0];                     // 32 dword = 128 byte
  BYTE NumRun;                                              // 1 byte
  BYTE INTHIST[128];                                        // 128 byte
  gmt_VideoInfoByte Image;                                  // 6 byte
  gmt_VideoInfoByte Rect[4];                                // 6*4 = 24 byte
  gmt_VideoInfoByte Rectcut[4];                             // 6*4 = 24 byte
  BYTE Start;                                               // 1 byte
}gmt_MotionInfo;                                            // => 5240 byte

typedef struct
{ 
  DWORD VWDarray[NUMBEROFXRGNS_0];
  DWORD VWDarrayUPV[NUMBEROFXRGNS_0];
  SWORD CalcBuff[NUMBEROFYRGNS_0+1][NUMBEROFXRGNS_0+1];
  BYTE  RectCol[NUMBEROFXRGNS_0+1];
  BYTE  RectRow[NUMBEROFYRGNS_0+1];
  BYTE  RectColStart[NUMBEROFXRGNS_0+1];
  BYTE  RectRowStart[NUMBEROFYRGNS_0+1];
  BYTE  RectColEnd[NUMBEROFXRGNS_0+1];
  BYTE  RectRowEnd[NUMBEROFYRGNS_0+1];
}gmt_FrameInfo;

typedef struct
{
  BYTE VWDarray[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];    // 30*32 = 960 byte
}gmt_PremInfo;                                        // => 960 byte

typedef struct
{
  //BYTE HistLast[VERTZOOM1NUMYRGNS*16];			//Coarse : To store Intensity Histogram of each Column
  //BYTE HistRow[HOZOOMNUMYRGNS*(HOZOOMYGAP+1)*XREGNSPERRUN_0*16];
  //BYTE IntensityCol[VERTZOOM1NUMYRGNS*(2*XREGNSPERRUN_0)];		//Coarse : Number of Real blocks of each Column
  //BYTE IntensityRow[(HOZOOMNUMYRGNS*(HOZOOMYSIZE+HOZOOMYGAP))*XREGNSPERRUN_0];
  //BYTE RealRegCol[NUMBEROFXRGNS_0];		//Coarse : Number of Real blocks of each Column
  //WORD RealRegCol[HOZOOMNUMYRGNS*(HOZOOMYGAP+1)];
  //WORD RealRegRow[HOZOOMNUMYRGNS*(HOZOOMYGAP+1)];
  BYTE TextureRow[HOZOOMNUMYRGNS+4];
  BYTE BlackRow[HOZOOMNUMYRGNS+4];
  DWORD TopBorderRow[HOZOOMNUMYRGNS][XREGNSPERRUN_0];
  DWORD BotBorderRow[HOZOOMNUMYRGNS][XREGNSPERRUN_0];
  DWORD LeftBorderCol[XREGNSPERRUN_0][VERTZOOM1NUMXRGNS];
  DWORD RightBorderCol[XREGNSPERRUN_0][VERTZOOM1NUMXRGNS];
  DWORD InSideBorder[HOZOOMNUMYRGNS][XREGNSPERRUN_0];
  DWORD INTXY[HOZOOMNUMYRGNS][VERTZOOM1NUMXRGNS];
  DWORD INT_RGB[3][HOZOOMNUMYRGNS][VERTZOOM1NUMXRGNS];
  BYTE Motion[HOZOOMNUMYRGNS][VERTZOOM1NUMXRGNS];
}gmt_FineFrameInfo;

typedef struct
{
  //DWORD INTXY[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];
  DWORD Motion[NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];
  DWORD RowMax[NUMBEROFYRGNS_0];
  DWORD ColMax[NUMBEROFXRGNS_0];
}gmt_DitherFrameInfo;

typedef struct
{
  DWORD INTXY[2][NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];
  BYTE Motion[2][NUMBEROFYRGNS_0][NUMBEROFXRGNS_0];
  BYTE YData[2][108][64];
  BYTE diff[2][64];
  BYTE Start;
  BYTE NumRun;
}gmt_DeIntFrameInfo;


enum
{
   STOP_ANALYZE,
   ANALYZE_ONCE,
   START_ANALYZE,
   POLL_ANALYZE,
   INTL_ANALYZE
};

enum // Detection Type
{
   DITHER_DETECTION = 0,
   COARSE_DETECTION = 1,
   COARSE_MVWSELECT = 2,
   FINE_DETECTION   = 3,
   STABLE_DETECTION = 4,
   BORDER_DETECTION = 5,
   BORDER_INSIDE    = 6,
   BORDER_STABLE    = 7
};

// State control
#define BLOCKRUN  0				// Run block search
#define BLOCKCALC 1				// Calc block search
#define TOPEDGE 1
#define BOTEDGE 2
#define LEFTEDGE 4
#define RIGHTEDGE 8 
#define INSIDE 10

//Run Type
enum
{
   RUN_EDGE    = 0,
   RUN_MOTION  = 1,
   RUN_REAL    = 2,
   RUN_R       = 3,
   RUN_G       = 4,
   RUN_B       = 5
};

//RECTANGLE TYPE
#define NO_RECTFOUND 0
#define PERFECT_RECTFOUND 1
#define CUT_RECTFOUND 2
#define NOTPERFECT_RECTFOUND 3

void PixCrDisable(void);

void PixCrAnalyzeImage(gmt_VideoInfo *videoInfo,
                       gmt_VideoInfo *coarsevideoInfo,
                       BYTE Mode,
                       gmt_PCInputConfig inputConfig,
                       BYTE *MainState,
                       gmt_MultiVideoInfo *CoarseMVWInfo,
                       BYTE *MVWON,
                       BYTE SelectRectext,
                       BYTE ExtSelectWR);

void PixCrAnalyzeACRStop(void);
void PixCrAnalyzeImageForACR(void);
WORD GetAvgINTXY(void);

