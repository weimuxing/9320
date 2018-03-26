/*
	$Workfile:   Version.c  $
	$Revision: 1.82 $
	$Date: 2013/03/05 01:43:35 $
*/
#define __VERSION_C__
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
// MODULE:      version.c
//
// USAGE:       This module contains the project version number and configuration
//              information.
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "string.h"
#include "System\all.h"

//******************************************************************************
//  E X T E R N A L
//******************************************************************************

extern ROM char gmc_AfeDecoderLibVersion[];
extern ROM char gmc_AfeDecoderLibDate[];
extern ROM char gmc_CC_LibVersion[];
extern ROM char gmc_CC_LibDate[];
extern ROM char gmc_TTX_LibVersion[];
extern ROM char gmc_TTX_LibDate[];
extern ROM char gmc_DRV_LibVersion[];
extern ROM char gmc_DRV_LibDate[];

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

// Date and Time
//
// 20110720, Added Athena Chip model
#ifdef Athena_U
//ROM char ChipName[] = "STDP93xx Athena U";
//ROM char ChipName[] = "U";
#endif
#ifdef Athena_C
//ROM char ChipName[] = "STDP93xx Athena C";
ROM char ChipName[] = "C";
#endif

#if 0
#if defined(EV3_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx EV3 Board";
#elif defined(EV1_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx EV1 Board";
#elif defined(EV2_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx EV2 Board";
#elif defined(RD3_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx RD3 Board";
#elif defined(RD2_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx RD2 Board";
#elif defined(RD1_93xx_BOARD)
ROM char ApplicationName[] = "STDP93xx RD1 Board";
#endif

#else
char ApplicationName[] = Board_ID;
ROM char Project_Name[] = ModeName;
#endif

//
// Version of FW
// Date and Time
//
#define MaxFirmwareVersionSize  14      // Firmware version number length

//ROM char ExtFirmwareVersion[MaxFirmwareVersionSize+1] = "1.1 RC0";

//ROM char DateTime[] = "Mar. 5, 2013 9:30 AM";
ROM char ExtFirmwareVersion[MaxFirmwareVersionSize+1] = CustomApp_FW_Version;
ROM char DateTime[] = CustomApp_FW_Date;

//
// *************  NOTE  *************
// Please change these numbers when the dates and version numbers are modified
// For OSD display purpose
//
//ROM BYTE B_VersionMaj = 2; 
//ROM BYTE B_VersionMin = 0;
//ROM BYTE B_RCNum      = 0;
ROM BYTE B_VerMonth   = 12;
ROM BYTE B_VerDate    = 21;
ROM BYTE B_VerYear    = 11;

//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void PrintVersion(void)
//
// USAGE          : Print applicaiton version number and system configuration
//                  options
//
// INPUT          :
//
// OUTPUT         : None
//
// GLOBALS        : gmc_SystemLibVersion	(RO)
//                  gmc_OsdLibVersion		(RO)
//                  gmc_SystemLibDate		(RO)
//                  gmc_OsdLibDate			(RO)
//					gmc_TTX_LibVersion		(RO)
//                  gmc_CC_LibVersion		(RO)
//					gmc_DRV_LibVersion		(RO)
//                  ApplicationName			(RO)
//                  ExtFirmwareVersion		(RO)
//                  DateTime				(RO)
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void PrintVersion(void)
{
   char C_DisplayBuf[40];

   gm_Print("*****",0);					  				// Line feed

#if(PRINT_IMPORTANT_MSG == ENABLE)
   //
   // System library version
   //
   _fstrcpy(C_DisplayBuf, (char *)"SYS Lib Ver - ");		//
   _fstrcat(C_DisplayBuf, (char *)gmc_SystemLibVersion);	//
   gm_Print(C_DisplayBuf,0);								//
   gm_Print(gmc_SystemLibDate,0);							//
   //
   // Osd library version
   //
   _fstrcpy(C_DisplayBuf, (const char *)"OSD Lib Ver - ");	//
   _fstrcat(C_DisplayBuf, (const char *)gmc_OsdLibVersion);//
   gm_Print(C_DisplayBuf,0);                           	//
   gm_Print(gmc_OsdLibDate,0);								//

   //
   // Driver library version
   //
   _fstrcpy(C_DisplayBuf, (const char *)"DRV Lib Ver - ");	//
   _fstrcat(C_DisplayBuf, (const char *)gmc_DRV_LibVersion);//
   gm_Print(C_DisplayBuf,0);                           	//
   gm_Print(gmc_DRV_LibDate,0);					  		//

   //
   // HDMI library version
   //
#if 0//(FEATURE_HDMI == ENABLE) //Sweng_Hdmi_Note
   {
      gmt_HDMI_LIB_VERSION ver = gm_HdmiGetLibVersion();
      gm_Print("HDMI %d.%d Lib Ver - %d.%d.%d.%d", ver.B_HdmiStdMajor, ver.B_HdmiStdMinor,
               ver.B_HdmiIpVersion, ver.B_HdmiIpRevision, ver.W_LibReleaseNumber, ver.W_LibBuildNumber);
   }
#endif	//FEATURE_HDMI == ENABLE)

#if 0//(FEATURE_DISPLAY_PORT == ENABLE)
   {
      gmt_DPRX_LibVersion ver = gm_DpRxGetLibVersion();
      gm_Print("DPRX %d.%d Lib Ver - %d.%d.%d.%d", ver.B_DpStdMajor, ver.B_DpStdMinor,
               ver.B_DpIpVersion, ver.B_DpIpRevision, ver.W_LibReleaseNumber, ver.W_LibBuildNumber);
   }
#endif //FEATURE_DISPLAY_PORT == ENABLE)

   //
   // CEC library version
   //
#if (FEATURE_CEC == ENABLE)
   {
      char	cecbuf[4];
      gmt_CEC_LIB_VERSION CecLibVersion;
      CecLibVersion = gm_CecGetLibVersion();

      _fstrcpy(C_DisplayBuf, (const char *)"CEC ");
      itoa (CecLibVersion.B_CecStdMajor,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (CecLibVersion.B_CecStdMinor,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)" Lib Ver - ");
      itoa (CecLibVersion.B_CecIpVersionMajor,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (CecLibVersion.B_CecIpVersionMinor,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (CecLibVersion.B_CecIpRevision,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (CecLibVersion.W_LibReleaseNumber,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (CecLibVersion.W_LibBuildNumber,&cecbuf[0],10);
      _fstrcat(C_DisplayBuf, &cecbuf[0]);
   }
   gm_Print(C_DisplayBuf,0);                           		//
#endif	//(FEATURE_CEC == ENABLE)
#endif//PRINT_IMPORTANT_MSG
   //
   // Application/board name
   //
   // 20110720, Print Chip model
   //gm_Print(ChipName,0);
   #ifdef UART_DEBUG_MODE
   gm_Print(ApplicationName,0);
   gm_Print(Project_Name,0);
   gm_Print(PanelName,0);								// Panel name
   #endif
   _fstrcpy(C_DisplayBuf, (const char *)"Application Ver - ");
   #if CHECK_ITEM
   {
      TXromConfigData far * pXromConfigData = (TXromConfigData far *)(((DWORD)W_SegEndExtRom<<16)+CONFIG_DATA_OFFSET);
      BYTE buf[5]={0};
      itoa (pXromConfigData->fw_major,buf,4);
      _fstrcat(C_DisplayBuf, buf);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (pXromConfigData->fw_minor,buf,4);
      _fstrcat(C_DisplayBuf, buf);
      _fstrcat(C_DisplayBuf, (const char *)".");
      itoa (pXromConfigData->fw_build,buf,4);
      _fstrcat(C_DisplayBuf, buf);
   }
   #else
   _fstrcat(C_DisplayBuf, (const char *)ExtFirmwareVersion);
   #endif
   gm_Print(C_DisplayBuf,0);							// Application version
#if CHECK_ITEM
   gm_Print(__DATE__", "__TIME__,0);
#else
   gm_Print(DateTime,0);								// Date-time
#endif

#if(PRINT_IMPORTANT_MSG == ENABLE)
#ifdef DRAM_CODE_EXECUTION
   #if (DRAM_CODE_EXECUTION_MODE == 1)
   if(SystemFlags.sys.DramCodeExecution == TRUE)
      gm_Print("DRAM code execution",0);
   #else
   gm_Print("DRAM code execution",0);
   #endif
#endif
#endif	//PRINT_IMPORTANT_MSG
	ZDTLib_Version();
   gm_Print(" ",0);					  				// Line feed
}

#ifdef Action_OsdFactoryMenuInfo_WB_Used
//******************************************************************************
//
// FUNCTION       : void OsdFactoryMenuInfo(void)
//
// USAGE          :
//
// INPUT          :
//
// OUTPUT         :
//
// GLOBALS        :
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void OsdFactoryMenuInfo(void)
{
   BYTE B_Param = gm_OsdGetActionByteParameter();
   TXromConfigData far * pXromConfigData = (TXromConfigData far *)(((DWORD)W_SegEndExtRom<<16)+CONFIG_DATA_OFFSET);

   switch (B_Param)
   {
      case 0:		//Major Version number
         UserPrefScrollMenuItem = pXromConfigData->fw_major; // B_VersionMaj;
         break;
      case 1:		//Minor Version number
         UserPrefScrollMenuItem = pXromConfigData->fw_minor; // B_VersionMin;
         break;
      case 2:		//RC number number
         UserPrefScrollMenuItem = pXromConfigData->fw_build; // B_RCNum;
         break;
      case 3:		//version month
         UserPrefScrollMenuItem = B_VerMonth;
         break;
      case 4:		//version date
         UserPrefScrollMenuItem = B_VerDate;
         break;
      case 5:		//version year
         UserPrefScrollMenuItem = B_VerYear;
         break;
   }
}
#endif
/*********************************  END  **************************************/
