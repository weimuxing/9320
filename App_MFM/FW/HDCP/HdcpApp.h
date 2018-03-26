/************************************************************************************************************

                                  COPYRIGHT (C) STMicroelectronics 2004.

	All rights reserved. This document contains proprietary and confidential information of the STMicro-
	electronics Group. This document is not to be copied in whole or part. STMicroelectronics assumes no
	responsibility for the consequences of use of such information nor for any infringement	of patents
	or other rights of third parties which may result from its use. No license is granted by implication
	or otherwise under any patent or patent rights of STMicroelectronics. STMicroelectronics products are
	not authorized for use as critical components in life support devices or systems without express writ-
	ten approval of STMicroelectronics.

File name 		:  HdcpApp.h
Author 			: 
Description		: 
Creation 		: 
*************************************************************************************************************/

#ifndef __HDCPAPP_H__
#define __HDCPAPP_H__


#define ENABLE_HDMIDVI_HDCP_REPEATER     0     

enum
{
	HdcpDpRepeater,
};

enum
{
	Dvi1RxPort		= iDpTxPort+1,
	Dvi2RxPort,
	Hdmi1RxPort,
	Hdmi2RxPort,
	Hdmi3RxPort,	
};


	 
BOOL far CreateRepeaterInstances(void);
//void far RunHdcpRep(void);

#endif
