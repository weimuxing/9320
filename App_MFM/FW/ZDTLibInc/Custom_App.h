#ifndef __CUSTOM_App_H__
#define __CUSTOM_App_H__

//
// RS485 mode
//
typedef enum RS485Enum
{
	RS_MODE_Listen,
	RS_MODE_Ctrl,
	RS_MODE_MAX,
} RS485Type;

//
// DPTX mode
//
typedef enum DPTX_Mode_Enum
{
	DPTX_iDP,
	DPTX_MST,
}DPTX_MODE_TYPE;

#ifdef PORT_CHANGE_SEAMLESS
typedef enum
{
	PCS_IDLE,
	PCS_INIT,
	PCS_Start,
	PCS_Step1,
	PCS_Step2,
	PCS_Step3,
		
	PCS_Done,	
	PCS_Max = PCS_Done,
} PortChangeStatesType;
#endif
enum
{
	ZM_OFF,
	ZM_Full,
	ZM_Normal,
	ZM_Dynamic
};


#if(defined(BOARD_93XX_PD)||defined(BOARD_93XX_OV))
BOOL SetFlexiblePxpWindowLimit(gmt_PHY_CH B_Channel,WORD Hstart, WORD Vstart, WORD Width, WORD Height);
BOOL SetFlexiblePxpWindow(gmt_PHY_CH B_Channel,WORD Hstart, WORD Vstart, WORD Width, WORD Height);
gmt_WindowStruct* GetFlexiblePxpWindow(gmt_PHY_CH B_Channel);
gmt_WindowStruct* GetFlexiblePxpWindowLimit(gmt_PHY_CH B_Channel);
void InitFlexibleWin(void);
void SavePDOriginalValue(gmt_PHY_CH B_Channel);
PDWindowStruct* GetPDOriginalValue(gmt_PHY_CH B_Channel);
void ModeSetupDisplayScaling(gmt_PHY_CH B_Channel, BYTE B_Step);
void AdjustPxpMode(void);
#endif
void AdjustPxpMode(void);
void SetMainOnTop(void);
void SetPxpOnTop(void);

void SetChannelOnTop(gmt_PHY_CH B_Channel);


#ifdef PORT_CHANGE_SEAMLESS
BOOL IsPortChangeSeamlessOK(void);
void SetPortChangeStates(PortChangeStatesType PC_State);
void PortChangeSeamlessHandler(void);
#endif
#if((defined(BOARD_93XX_PD))||(defined(BOARD_93XX_SUV))||(defined(BOARD_93XX_OV)))
void SetRS485Mode(RS485Type RS_Mode);
#endif
void AdjustOutputMode(void);
void PanelArraySelect(void);
BOOL IsDecoderInputStable(gmt_PHY_CH B_Channel);
void SetDPOutputMode(DPTX_MODE_TYPE DPTX_MODE);
#ifdef OUTPUT_MODE_CHANGE
void StartChangeOutputMode(void);
//BOOL OutputModeAutoChange(gmt_PHY_CH B_Channel);
//BOOL OutputModeAutoDetermine(gmt_PHY_CH B_Channel);
void OutputModeHandler(gmt_PHY_CH B_Channel);
#endif

void AdjustMainBlendLevel(void);
void AdjustMainMode(void);
void AdjustMainPosition(void);
WORD GetStatusFromLPM(BYTE CMD_Head, BYTE CMD_Value1, BYTE CMD_Value2);
void SendCMDToLPM(BYTE CMD_Head, BYTE CMD_Value1, BYTE CMD_Value2);
void AdjustZoomMode(gmt_PHY_CH B_Channel, BYTE ZM_Mode, BYTE ZM_Percent, BYTE B_Step);	//Neil 20081218* this function will be called by mode setup to calculate the relative registers;
void ModeSetupDynamicScaling_App(gmt_PHY_CH B_Channel, WORD Param, BYTE B_Step);
void ReInit_AVDDS(gmt_PHY_CH B_Channel);
#ifdef CHANNEL_CLONE
BOOL IsCloneAvariable(gmt_PHY_CH B_Channel);
#endif
BOOL IsPxpAvariable(void);

BOOL Is_PD_ON(gmt_PHY_CH B_Channel);

#ifdef ENABLE_FAN_CONTROL
void FAN_TEMP_Handler(void);
#endif


#ifdef ENABLE_REALTIME_CONTROL



void RTC_GetCurrentTimeAll(void);
void RTC_SetCurrentTimeAll(void);
void RTC_GetCurrentTime(RTC_TimeType RTC_TIME);
void RTC_SetCurrentTime(RTC_TimeType RTC_TIME, WORD RTC_Value);
void RTC_GetAlarmTimeAll(void);
void RTC_GetAlarmTime(RTC_TimeType RTC_TIME);
void RTC_SetAlarmTime(RTC_TimeType RTC_TIME, BYTE RTC_Value, BOOL YesNo);
void RTC_InitTime(void);

#if 0
BYTE RTC_Read(BYTE W_RegAddr);
BYTE RTC_Write(BYTE W_RegAddr, BYTE B_Data);
BOOL RTC_CheckDevice(void);
#endif
#endif

/***********************************  END  ************************************/
#endif
