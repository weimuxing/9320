/*
	$Workfile:   GammaLut.h  $
	$Revision: 1.6 $
	$Date: 2012/03/29 07:25:55 $
*/

/******************************************************************************
				G A M M A    L U T
/*****************************************************************************/
//#include "..\..\Panel\1080p\CMO\M236H5_4K2K\xvYCC_6500_PreLUT.h"
//#include "..\..\Panel\1080p\CMO\M236H5_4K2K\xvYCC_6500_PostLUT.h"
//#include "..\..\Panel\1080p\CMO\M236H5_4K2K\xvYCC_6500_PostLUT_Steep.h"
//#include "..\..\Panel\1080p\CMO\M236H5_4K2K\xvYCC_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_6500_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_6500_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_6500_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_7500_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_7500_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_7500_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_7500_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_9300_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_9300_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_9300_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\sRGB_9300_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_6500_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_6500_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_6500_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_7500_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_7500_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_7500_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_7500_colorMappingGamutMatrix.h"

#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_9300_PreLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_9300_PostLUT.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_9300_PostLUT_Steep.h"
#include "..\..\Panel\1080p\CMO\M236H5_4K2K\AdobeRGB98_9300_colorMappingGamutMatrix.h"

WORD ROM LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K[3]=
{
   3874, // R
   3871, // G
   4584  // B
};

gmt_GammaDescr ROM xvYCC_6500K_M236H5_4K2K =
{
	ColorSpace_XvYcc,
	CTEMP_6500K,
	{
		xvYCC_6500K_RED_PreLUT_LM240WU4_SLB1,
		xvYCC_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		xvYCC_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM sRGB_6500K_M236H5_4K2K =
{
	ColorSpace_sRGB,
	CTEMP_6500K,
	{
		sRGB_6500K_RED_PreLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PreLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM sRGB_7500K_M236H5_4K2K =
{
	ColorSpace_sRGB,
	CTEMP_7500K,
	{
		sRGB_6500K_RED_PreLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PreLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_M236H5_4K2K, // COLOR_MAPPING_GAMUT_MATRIX_sRGB_7500K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM sRGB_9300K_M236H5_4K2K =
{
	ColorSpace_sRGB,
	CTEMP_9300K,
	{
		sRGB_6500K_RED_PreLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PreLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_M236H5_4K2K, // COLOR_MAPPING_GAMUT_MATRIX_sRGB_9300K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM AdobeRGB98_6500K_M236H5_4K2K =
{
	ColorSpace_Adobe98,
	CTEMP_6500K,
	{
		AdobeRGB98_6500K_RED_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_GREEN_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM AdobeRGB98_7500K_M236H5_4K2K =
{
	ColorSpace_Adobe98,
	CTEMP_7500K,
	{
		AdobeRGB98_6500K_RED_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_GREEN_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_M236H5_4K2K, // COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_7500K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};


gmt_GammaDescr ROM AdobeRGB98_9300K_M236H5_4K2K =
{
	ColorSpace_Adobe98,
	CTEMP_9300K,
	{
		AdobeRGB98_6500K_RED_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_GREEN_PreLUT_M236H5_4K2K,
		AdobeRGB98_6500K_BLUE_PreLUT_M236H5_4K2K
	},
	{
		sRGB_6500K_RED_PostLUT_M236H5_4K2K,
		sRGB_6500K_GREEN_PostLUT_M236H5_4K2K,
		sRGB_6500K_BLUE_PostLUT_M236H5_4K2K
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_M236H5_4K2K, // COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_9300K_M236H5_4K2K,
	0, // LUT1 Steep On/Off
	0, // LUT1 Steep Step
	0, // LUT1 Steep Start
	0, // LUT1 Steep End
	0, // LUT1 Steep Final
   {  // LUT1 Steep Table
      0,
      0,
      0
   },
	1, // LUT2 Steep On/Off
	2, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0x1FF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_M236H5_4K2K, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_GREEN_PostLUT_Steep_M236H5_4K2K,
      sRGB_6500K_BLUE_PostLUT_Steep_M236H5_4K2K
   },	
};

gmt_GammaDescr ROM * ROM Sta_GammaLUT_M236H5_4K2K[] =
{
	&xvYCC_6500K_M236H5_4K2K,
	&sRGB_6500K_M236H5_4K2K,
	&sRGB_7500K_M236H5_4K2K,
	&sRGB_9300K_M236H5_4K2K,
	&AdobeRGB98_6500K_M236H5_4K2K,
	&AdobeRGB98_7500K_M236H5_4K2K,
	&AdobeRGB98_9300K_M236H5_4K2K,	
	NULL_PTR						// Signifies no more gamma tables	
};
