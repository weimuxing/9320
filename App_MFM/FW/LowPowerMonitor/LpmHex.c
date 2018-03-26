//******************************************************************************
//
//              Copyright (C) 2008.  GENESIS MICROCHIP INC. CONFIDENTIAL
//      All rights reserved.  No part of this program may be reproduced.
//
//     Genesis Microchip Inc., 165 Commerce Valley Dr. West
//     Thornhill, Ontario, Canada, L3T 7V8
//
//==============================================================================
//
// MODULE:    LpmHex.c
//
// USAGE:     
//
//******************************************************************************
#include "System\all.h"

#if(defined(LPM_UART_SUPPORT))
	#include "LowPowerMonitor\LpmSTDP93xx_hex_Debug.c"
#else
	#ifdef EXTERNAL_DECODER
		#if(DECODER_IN_LPM == 1)
			#ifdef ENABLE_REALTIME_CONTROL
				#include "LowPowerMonitor\LpmSTDP93xx_hex_Video_RTC.c"
			#else
				#include "LowPowerMonitor\LpmSTDP93xx_hex_Video.c"
			#endif
		#else
			#ifdef ENABLE_REALTIME_CONTROL
				#include "LowPowerMonitor\LpmSTDP93xx_hex_NoVideo_NoTouch_RTC.c"
			#else
				#include "LowPowerMonitor\LpmSTDP93xx_hex_NoVideo_NoTouch.c"
			#endif
		#endif
	#elif(defined(TOUCH_KEY))	//(LOGO_SELECT == LOGO_DONVIEW)
		#ifdef TOUCHKEY_INVERSE
		#include "LowPowerMonitor\LpmSTDP93xx_hex_Touch_INVERSE.c"

		#else
		#include "LowPowerMonitor\LpmSTDP93xx_hex_Touch.c"

		#endif
	#else
		#ifdef ENABLE_REALTIME_CONTROL
			#include "LowPowerMonitor\LpmSTDP93xx_hex_NoVideo_NoTouch_RTC.c"
		#else
			#include "LowPowerMonitor\LpmSTDP93xx_hex_NoVideo_NoTouch.c"
		#endif
	#endif
#endif

//*********************************  END  **************************************
