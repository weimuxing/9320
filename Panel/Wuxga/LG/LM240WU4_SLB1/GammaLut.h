/*
	$Workfile:   GammaLut.h  $
	$Revision: 1.8 $
	$Date: 2012/03/29 07:25:55 $
*/

/******************************************************************************
				G A M M A    L U T
/*****************************************************************************/
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_6500_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_6500_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_6500_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_7500_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_7500_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_7500_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_7500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_9300_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_9300_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_9300_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\sRGB_9300_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_6500_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_6500_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_6500_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_7500_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_7500_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_7500_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_7500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_9300_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_9300_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_9300_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\AdobeRGB98_9300_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\xvYCC_6500_PreLUT.h"
//#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\xvYCC_6500_PostLUT.h"
//#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\xvYCC_6500_PostLUT_Steep.h"
//#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\xvYCC_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_6500_PreLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_6500_PostLUT.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_6500_PostLUT_Steep.h"
#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_6500_colorMappingGamutMatrix.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_7500_PreLUT.h"

#include "..\..\Panel\WUxga\LG\LM240WU4_SLB1\DICOM_9300_PreLUT.h"


WORD ROM LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1[3]=
{
/*
   8693, // R
   8366, // G
   8478, // B
*/
   2173, // R
   2092, // G
   2120  // B
};

WORD ROM LUT2_Steep_Final_DICOM_6500K_LM240WU4_SLB1[3]=
{
   2285, // 2337, // R
   2285, // G
   2285, // 2385  // B
};

gmt_GammaDescr ROM xvYCC_6500K_LM240WU4_SLB1 =
{
	ColorSpace_XvYcc,
	CTEMP_6500K,
	{
		xvYCC_6500K_RED_PreLUT_LM240WU4_SLB1,
		xvYCC_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		xvYCC_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};


gmt_GammaDescr ROM sRGB_6500K_LM240WU4_SLB1 =
{
	ColorSpace_sRGB,
	CTEMP_6500K,
	{
		sRGB_6500K_RED_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM sRGB_7500K_LM240WU4_SLB1 =
{
	ColorSpace_sRGB,
	CTEMP_7500K,
	{
		sRGB_6500K_RED_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_LM240WU4_SLB1, // COLOR_MAPPING_GAMUT_MATRIX_sRGB_7500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM sRGB_9300K_LM240WU4_SLB1 =
{
	ColorSpace_sRGB,
	CTEMP_9300K,
	{
		sRGB_6500K_RED_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_sRGB_6500K_LM240WU4_SLB1, // COLOR_MAPPING_GAMUT_MATRIX_sRGB_9300K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM AdobeRGB98_6500K_LM240WU4_SLB1 =
{
	ColorSpace_Adobe98,
	CTEMP_6500K,
	{
		AdobeRGB98_6500K_RED_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM AdobeRGB98_7500K_LM240WU4_SLB1 =
{
	ColorSpace_Adobe98,
	CTEMP_7500K,
	{
		AdobeRGB98_6500K_RED_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_LM240WU4_SLB1, // COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_7500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};


gmt_GammaDescr ROM AdobeRGB98_9300K_LM240WU4_SLB1 =
{
	ColorSpace_Adobe98,
	CTEMP_9300K,
	{
		AdobeRGB98_6500K_RED_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		AdobeRGB98_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		sRGB_6500K_RED_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		sRGB_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_6500K_LM240WU4_SLB1, // COLOR_MAPPING_GAMUT_MATRIX_AdobeRGB98_9300K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_sRGB_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      sRGB_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      sRGB_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM DICOM_6500K_LM240WU4_SLB1 =
{
	ColorSpace_DICOM,
	CTEMP_6500K,
	{
		DICOM_6500K_RED_PreLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PreLUT_LM240WU4_SLB1,
		DICOM_6500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_DICOM_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_DICOM_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM DICOM_7500K_LM240WU4_SLB1 =
{
	ColorSpace_DICOM,
	CTEMP_7500K,
	{
		DICOM_7500K_RED_PreLUT_LM240WU4_SLB1,
		DICOM_7500K_GREEN_PreLUT_LM240WU4_SLB1,
		DICOM_7500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_DICOM_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_DICOM_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM DICOM_9300K_LM240WU4_SLB1 =
{
	ColorSpace_DICOM,
	CTEMP_9300K,
	{
		DICOM_9300K_RED_PreLUT_LM240WU4_SLB1,
		DICOM_9300K_GREEN_PreLUT_LM240WU4_SLB1,
		DICOM_9300K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_DICOM_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_DICOM_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};

gmt_GammaDescr ROM DICOM_USER_LM240WU4_SLB1 =
{
	ColorSpace_DICOM,
	CTEMP_USER,
	{
		DICOM_7500K_RED_PreLUT_LM240WU4_SLB1,
		DICOM_7500K_GREEN_PreLUT_LM240WU4_SLB1,
		DICOM_7500K_BLUE_PreLUT_LM240WU4_SLB1
	},
	{
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1,
		DICOM_6500K_GREEN_PostLUT_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_LM240WU4_SLB1
	},	
	COLOR_MAPPING_GAMUT_MATRIX_DICOM_6500K_LM240WU4_SLB1,
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
	3, // LUT2 Steep Step
	0, // LUT2 Steep Start
	0xFF, // LUT2 Steep End
	LUT2_Steep_Final_DICOM_6500K_LM240WU4_SLB1, // LUT2 Steep Final
   {  // LUT2 Steep Table
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_RED_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1,
      DICOM_6500K_GREEN_PostLUT_Steep_LM240WU4_SLB1, // DICOM_6500K_BLUE_PostLUT_Steep_LM240WU4_SLB1
   },	
};



gmt_GammaDescr ROM * ROM Sta_GammaLUT_LM240WU4_SLB1[] =
{
   &sRGB_6500K_LM240WU4_SLB1,
   &sRGB_7500K_LM240WU4_SLB1,
   &sRGB_9300K_LM240WU4_SLB1,
   &AdobeRGB98_6500K_LM240WU4_SLB1,
   &AdobeRGB98_7500K_LM240WU4_SLB1,
   &AdobeRGB98_9300K_LM240WU4_SLB1,   
   &xvYCC_6500K_LM240WU4_SLB1,   
   &DICOM_6500K_LM240WU4_SLB1,
   &DICOM_7500K_LM240WU4_SLB1,
   &DICOM_9300K_LM240WU4_SLB1,
   //&DICOM_USER_LM240WU4_SLB1,   
   NULL_PTR						// Signifies no more gamma tables	
};

