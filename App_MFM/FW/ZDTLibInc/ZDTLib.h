#ifndef __ZDTLib_H__
#define __ZDTLib_H__


//define for library 
//#define UART_DEBUG_MODE

#if(defined(BOARD_93XX_4K2K))
#define DYNAMIC_OUTPUT				//DynamicOutput
#endif


typedef enum
{
	DE_3840x2160,		//3840x2160 set as default;
	DE_1366x768,
	DE_1920x1080,
	DE_1920x1200,
	DE_2560x1080,
	DE_2560x1440,
	DE_2560x1600,
	DE_Max,
}DynamicEDIDType;


typedef enum
{
	EB_Header = 0x07,
	EB_ID_Name =0x08,		//0x08, 2Bytes
	EB_ID_ProductCode = 0x0A,	//0x0A, 2Bytes
	EB_SN_String = 0x0C,		//0x0Ch, 4Bytes
	EB_MFT_Week = 0x10,		//0x10h, 1Bytes,
	EB_MFT_Year = 0x11,		//0x11h, 1Bytes,
	EB_Versiong = 0x12, 		//0x12h, 2Bytes,
	EB_DisplayFeature = 0x13, 		//0x13h, 5Bytes, basic display parameter/feature
	EB_DTD_Timing = 0x36,				//0x36, 18Bytes, 	Detailed Timing Dscription. 	default timing
	EB_DTD_SN = 0x48,					//0x48, 18Bytes,	Detailed Timing Dscription.	SN
	EB_DTD_Name = 0x5A,					//0x5A,18Bytes,	Detailed Timing Dscription.	Name
	EB_All = 0xFF,				//All EDID Data,
}EDIDBlockType;

typedef enum RTCEnum
{
   RTC_READ,
   RTC_WRITE,
}RTC_Type;

typedef enum
{
	RTC_Sec = 0x02,
	RTC_Min,		//0x03
	RTC_Hour,		//0x04
	RTC_Day,		// 0x05
	RTC_Week,		//0x06
	RTC_Month,		//0x07
	RTC_Year,		//0x08
	RTC_Alarm_Min = 0x09, 	//0x09
	RTC_Alarm_Hour,	//0x09
	RTC_Alarm_Day,	//0x09
	RTC_Alarm_Week,	//0x09

	RTC_All = 0xF0,	// 0x
	RTC_Alarm_All = 0xF8,	// 0x
}RTC_TimeType;


typedef struct RTC_TimeStruct
{
	BYTE T_Sec;
	BYTE T_Min;
	BYTE T_Hour;
	BYTE T_Day;
	BYTE T_Week;
	BYTE T_Month;
	BYTE T_Year;
}RTC_TIME_TYPE;


#ifdef EXTERNAL_DECODER
// Init default
#define MAX_VD_BRI		0xFF
#define MIN_VD_BRI		0
#define MAX_VD_CON		172
#define MIN_VD_CON		92
#define MAX_VD_SAT		0xFF
#define MIN_VD_SAT		0
#define MAX_VD_HUE		0xFF
#define MIN_VD_HUE		0


//#if DECODER_IN_LPM
typedef enum VideoDriverFuncEnum
{
   gmd_VID_SET_INPUT,
   gmd_VID_SET_OUTPUT_FORMAT,
   gmd_VID_SET_MODE,
   gmd_VID_SET_MIXER,
   gmd_VID_SET_BRIGHTNESS,
   gmd_VID_SET_CONTRAST,		// 5
   gmd_VID_SET_SATURATION,
   gmd_VID_SET_HUE,
   gmd_VID_SET_SHARPNESS,
   gmd_VID_GET_STATUS,
   gmd_VID_GET_INTERLACE,		// 10
   gmd_VID_GET_LINES_PER_FIELD,
   gmd_VID_GET_SUB_MODE,
   gmd_VID_GET_MODE,
   gmd_VID_PWR_UP,
   gmd_VID_PWR_DOWN,			// 15
   gmd_VID_RAW_READ,
   gmd_VID_RAW_WRITE,
   gmd_VID_SET_TWOWIRE_PORT,
   gmd_VID_GET_CHIPID,
   gmd_VID_CheckOutput,			// 20
	   
}VideoDriverFuncType;

#define VDD_MODE_NOVIDEO       0
#define VDD_MODE_NTSC          1
#define VDD_SUBMODE_NTSC_M     2
#define VDD_SUBMODE_NTSC_44    3
#define VDD_SUBMODE_PAL_M      4
#define VDD_SUBMODE_PAL_60     5
#define VDD_SUBMODE_NTSC_COMB   6
#define VDD_SUBMODE_PAL_BGHI   7
#define VDD_SUBMODE_PAL_N      8
#define VDD_MODE_PAL         	9
#define VDD_SUBMODE_SECAM      10
#define VDD_MODE_SECAM         11
#define VDD_MODE_INIT         12
#define VDD_MODE_CHANGE       13
#define VDD_MODE_480p         			14
#define VDD_MODE_576p         			15



#define NO_HS							BIT1
#define NO_VS							BIT2
#define HS_VS_MASK						(NO_HS|NO_VS)

// Video types
#define VDD_INTYP_NONE          				0
#define VDD_INTYP_COMPOSITE1    			1
#define VDD_INTYP_SVIDEO        				2
#define VDD_INTYP_COMPOSITE2    			3
#define VDD_INTYP_TUNER         				4
#define VDD_INTYP_COMPONENT     			5

#define D_NOTLOCKED             	0//0
#define D_LOCKED				1
#define D_ModeChange			2

#define D_NONINTERLACED         0
#define D_INTERLACED 			1


#endif

void ZDTLib_Version(void);

void SetNoSyncOSDByPanelParameters(void);
void ResetMission(void);

#endif

