#ifndef __CUSTOM_4K2K_H__
#define __CUSTOM_4K2K_H__


typedef enum
{
   T1920x1080_48Hz,
   T1920x1080_50Hz,
   T1920x1080_60Hz,
   T3840x2160_24Hz,
   T3840x2160_30Hz,
} t_AthenaOutputTiming;

#define WB_CONTRAST_MAX				100//306 //384 
#define WB_CONTRAST_MIN				0//206 //128 


#if 1//(DEFAULT_PANEL == ZDT_4K2K_CMI)


gmt_RET_STAT ZDT_4K2K_CMI_Panel_FHD120(gmd_MasterPanelList PanelIndex,BYTE B_Mode);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_FHD60(gmd_MasterPanelList PanelIndex,BYTE B_Mode);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_QFHD30(gmd_MasterPanelList PanelIndex,BYTE B_Mode);
#ifndef CMI_8921A		// for 8903
gmt_RET_STAT ZDT_4K2K_CMI_Panel_MuteOff(gmd_MasterPanelList PanelIndex);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_MuteOn(gmd_MasterPanelList PanelIndex);
#endif
gmt_RET_STAT ZDT_4K2K_CMI_Panel_Blank(gmd_MasterPanelList PanelIndex,BOOL YesOrNo);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_LVDS_JD8(gmd_MasterPanelList PanelIndex);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_LVDS_JD10(gmd_MasterPanelList PanelIndex);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_LVDS_VS8(gmd_MasterPanelList PanelIndex);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_LVDS_VS10(gmd_MasterPanelList PanelIndex);

gmt_RET_STAT ZDT_4K2K_CMI_Panel_MEMC_ADJ_LEVEL(gmd_MasterPanelList PanelIndex,BYTE B_AdjValue);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_MEMC_DEMO(gmd_MasterPanelList PanelIndex,BYTE SR_DEMO);


gmt_RET_STAT ZDT_4K2K_CMI_Panel_SR_ADJ_LEVEL(gmd_MasterPanelList PanelIndex,BYTE B_AdjValue);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_SR_DEMO(gmd_MasterPanelList PanelIndex,BYTE SR_DEMO);


gmt_RET_STAT ZDT_4K2K_CMI_Panel_ADJ_GAIN_OFFSET(gmd_MasterPanelList PanelIndex,BYTE B_Color, BYTE B_AdjValueLo, BYTE B_AdjValueHi);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_ADJ_SHAPNESS(gmd_MasterPanelList PanelIndex,BYTE B_AdjValue);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_ADJ_BRIGHTNESS(gmd_MasterPanelList PanelIndex,BYTE R_AdjValue,BYTE G_AdjValue,BYTE B_AdjValue);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_ADJ_CONTRAST(gmd_MasterPanelList PanelIndex,BYTE B_AdjValue);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_LD_DEMO(gmd_MasterPanelList PanelIndex,BYTE LD_DEMO);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_ColorEngine(gmd_MasterPanelList PanelIndex,BOOL YesOrNo);
//gmt_RET_STAT ZDT_4K2K_CMI_Panel_UnKnown01(void);
//gmt_RET_STAT ZDT_4K2K_CMI_Panel_UnKnown02(void);
//#elif((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))

gmt_RET_STAT ZDT_4K2K_CMI_Panel_RPF(gmd_MasterPanelList PanelIndex,BOOL YesNo);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_AUTOBLANK(gmd_MasterPanelList PanelIndex,BYTE B_DELAY);
#endif	//(DEFAULT_PANEL == ZDT_4K2K_CMI)

#if 1//((DEFAULT_PANEL == ZDT_4K2K_CMI_39)||(DEFAULT_PANEL == ZDT_4K2K_CMI)||(DEFAULT_PANEL == ZDT_4K2K_CMI_65))
void ZDT_4K2K_CMI_Panel_Init(gmd_MasterPanelList PanelIndex);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_WriteData(BYTE far*Bp_Data, WORD W_Len);
gmt_RET_STAT ZDT_4K2K_CMI_Panel_INX_WriteData(BYTE far*Bp_Data, WORD W_Len);
#endif

#if 0//def OUTPUT_MODE_CHANGE
void PanelArraySelect_4K2K(void);
BOOL OutputResolutionAutoDetermine(gmt_PHY_CH B_Channel);
void StartChangeOutputMode_4K2K(void);
void OutputModeHandler_4K2K(gmt_PHY_CH B_Channel);
#endif
#if 1//(DEFAULT_PANEL == ZDT_4K2K_QFHD)
void ZD_4K2K_InitPAChip(void);
#endif
void ReConfigLVDSSetting(WORD PanelW);

#endif
