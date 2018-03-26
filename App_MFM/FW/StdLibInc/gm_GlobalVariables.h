/*
            $Workfile:   gm_GlobalVariables.h  $
            $Revision: 1.7 $
            $Date    :   $
*/

#ifndef __GMGLOBALVARIABLE_H__
#define __GMGLOBALVARIABLE_H__
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
// MODULE:      gmglobalvariables.h
//
// USAGE:       This module contains all the Global variables used across all
//		the modules in the library. This is the header file containing
//		the externs declarations
//
//******************************************************************************

//******************************************************************************
//  E X T E R N S
//******************************************************************************
extern ROM char 			gmc_SystemLibVersion[];					// System Library version number
extern ROM char 			gmc_SystemLibDate[];					// System Library Date

// *** IROM   ******************************************************************
extern far	WORD			gmvw_FilmModeState;						// Read only flag
//extern far	BYTE			gmvb_DebugOnly;       					// Read only flag
//extern far	WORD			gmvw_GProbeFlag;

// *****************************************************************************
extern gmt_SOURCE_TYPE		gmvb_ChannelSource[NUM_CHANNELS];       // Channel Source (DIP_A, DIP_B, AIP_HD_GFX, AIP_SD_VID, AIP_FB_OVL, STG_TIM)
extern gmt_InputStruct	    stInput[NUM_CHANNELS];
extern gmt_SYNC_FLAGS		gmvw_InputSyncFlags[NUM_CHANNELS];
extern WORD 				gmvw_InputOversample[NUM_CHANNELS];		//Count
extern BYTE					gmvb_InputSamplingPhase[NUM_CHANNELS];
extern SWORD 				gmvsw_InputHSyncOffset[NUM_CHANNELS];	//Pixels
extern SWORD 				gmvsw_InputVSyncOffset[NUM_CHANNELS];	//Lines
extern WORD 				gmvw_InputVTotalOrg[NUM_CHANNELS];		//Lines
extern WORD 				gmvw_InputHTotalOrg[NUM_CHANNELS];		//Pixels
extern WORD 				gmvw_InputVStartOrg[NUM_CHANNELS];		//Lines - Original Input Parameters from ModeDeterm (Table)
extern WORD 				gmvw_InputVActiveOrg[NUM_CHANNELS];		//Lines
extern WORD 				gmvw_InputHStartOrg[NUM_CHANNELS];		//Pixel
extern WORD 				gmvw_InputHActiveOrg[NUM_CHANNELS];		//Pixels
extern gmt_SYNC_TYPE 		gmvb_CurrentSyncState[CH_PHY_NUM];
extern BOOL 				gmvb_EnableClamp[NUM_ADC_SET];
extern BOOL 				gmvb_SyncDetectClamp[NUM_ADC_SET];
extern volatile WORD		gmvb_VSyncEvent[NUM_CHANNELS];		    // SW latch for IMP_VS, IPP_VS
extern BYTE					gmvb_MemoryBpp[NUM_CHANNELS];			//Bits per pixel
extern BYTE					gmvb_MemoryNumBuffers[NUM_CHANNELS];	//Num memory buffers
extern gmt_BUFF_TYPE		gmvb_MemoryBufferTypes[NUM_CHANNELS];	//Memory buffer types allocated
extern WORD					gmvw_MemoryMaxHCapture[NUM_CHANNELS];//Pixels
extern WORD					gmvw_MemoryMaxVCapture[NUM_CHANNELS];	//Pixels
extern WORD 				gmvw_OutputVActive[NUM_CHANNELS];		// Lines
extern WORD 				gmvw_OutputVStart[NUM_CHANNELS];		// Line
extern WORD 				gmvw_OutputHActive[NUM_CHANNELS];		// Pixels
extern WORD 				gmvw_OutputHStart[NUM_CHANNELS];		// Pixel
extern BYTE					gmvb_OutputFrameRate;					// Hz (60/75/etc)
extern BYTE					gmvb_ValidIrData;						// IR Valid Data
extern gmt_PHY_CH 			gmvb_MainChannel;			            // Channel assigned to Main (CH_A)
extern gmt_PHY_CH 			gmvb_PipChannel;		 	            // Channel assigned to PIP (CH_B)
extern gmt_PHY_CH			gmvb_DDDSSourceChannel;	 	            // Channel DDDS is Locked to
extern gmt_HARDWARE_UPDATE_TASK 	gmvw_HwUpdateCtrl;		   	    // Enable all handlers
extern BYTE 				gmvb_STABLE_COUNT[NUM_CHANNELS]; 		// (W) Input stable threshold counter
extern BYTE 				gmvb_UNSTABLE_COUNT[NUM_CHANNELS];		// (W) Input unstable threshold counter
extern BYTE					gmvb_InputConnector[NUM_CHANNELS];
extern gmt_VIDEO_PROCESSING	gmvd_VideoProcessingDesired;			// Video Processing Modes Requested by Application
extern gmt_VIDEO_PROCESSING	gmvd_VideoProcessingEnabled;			// Video Processing Modes enabled by Library
extern gmt_ACC_ACM_MODE     gmvw_AccAcmMode;					    // Acc / Acm Modes
extern gmt_AdaptiveTnrSharpNoiseMethods gmvb_AdaptiveTNRMethod;	    // VQLAB_MAY07_METHOD = 0, CORTEZ27REL_METHOD = 1
extern BYTE					gmvb_VCRMode[NUM_CHANNELS];				// VCR Detection
extern WORD					gmv_OUTPUT_HTOTAL;
extern WORD					gmv_OUTPUT_VTOTAL;
extern WORD					gmv_OUTPUT_HTOTAL_22;
extern WORD					gmv_OUTPUT_VTOTAL_22;
extern WORD					gmv_OUTPUT_HTOTAL_33;
extern WORD					gmv_OUTPUT_VTOTAL_33;
extern WORD					gmv_OUTPUT_HACTIVE;						//Pixels
extern WORD					gmv_OUTPUT_VACTIVE;						//Lines
extern gmt_PanelSetupExeptions gmv_PanelSetupExeptions;//bit feeld to read into library panel exceptions;

#if (IMP_VS_MASK == IPP_VS_MASK)
#define ip_vs_mask IMP_VS_MASK
#endif
#if (IMP_VS == IPP_VS)
#define ip_vs IMP_VS
#endif
#if (IMP_PIXGRAB_EN == IPP_PIXGRAB_EN)
#define ip_pixgrab_en IMP_PIXGRAB_EN
#endif
#if (IMP_LINEFLAG == IPP_LINEFLAG)
#define ip_lineflag IMP_LINEFLAG
#endif
#if (IMP_INTLC_ERR == IPP_INTLC_ERR)
#define ip_intlc_err IMP_INTLC_ERR
#endif
#if (IMP_HS_INV == IPP_HS_INV)
#define ip_hs_inv IMP_HS_INV
#endif
#if (IMP_VS_INV == IPP_VS_INV)
#define ip_vs_inv IMP_VS_INV
#endif

extern WORD ROM ip_hs_delay[CH_PHY_NUM];
extern WORD ROM ip_h_act_start[CH_PHY_NUM];
extern WORD ROM ip_h_act_width[CH_PHY_NUM];
extern WORD ROM ip_vmeasure_start[CH_PHY_NUM];
extern WORD ROM ip_vmeasure_length[CH_PHY_NUM];
extern WORD ROM ip_v_act_start_odd[CH_PHY_NUM];
extern WORD ROM ip_v_act_start_even[CH_PHY_NUM];
extern WORD ROM ip_v_act_length[CH_PHY_NUM];
extern WORD ROM ip_hmeasure_start[CH_PHY_NUM];
extern WORD ROM ip_hmeasure_width[CH_PHY_NUM];
extern WORD ROM ip_flagline[CH_PHY_NUM];
extern WORD ROM ip_pixgrab_v[CH_PHY_NUM];
extern WORD ROM ip_pixgrab_h[CH_PHY_NUM];
extern WORD ROM ip_control[CH_PHY_NUM];
extern WORD ROM ip_ibd_hwidth[CH_PHY_NUM];
extern WORD ROM ip_ibd_hstart[CH_PHY_NUM];
extern WORD ROM ip_ibd_vlength[CH_PHY_NUM];
extern WORD ROM ip_ibd_vstart[CH_PHY_NUM];
extern WORD ROM ip_ibd_control[CH_PHY_NUM];
extern WORD ROM ip_ibd_htotal[CH_PHY_NUM];
extern WORD ROM ip_ibd_vtotal[CH_PHY_NUM];
extern WORD ROM input_irqmask[CH_PHY_NUM];
extern WORD ROM input_status[CH_PHY_NUM];
extern WORD ROM ip_odd_status[CH_PHY_NUM];
extern WORD ROM ip_pixgrab_red[CH_PHY_NUM];
extern WORD ROM ip_pixgrab_grn[CH_PHY_NUM];
extern WORD ROM ip_pixgrab_blu[CH_PHY_NUM];
extern WORD ROM ip_vs_status[CH_PHY_NUM];
extern WORD ROM ip_source[CH_PHY_NUM];
extern WORD ROM ip_vs_delay[CH_PHY_NUM];

#endif //__GMGLOBALVARIABLE_H__

