/*
	$Workfile:   ISR_RS_signals_transfer.c  $
	$Revision: 1.5 $
	$Date: 2012/07/09 06:33:49 $
*/

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
// MODULE:      gm_VBIInit.c
//
// USAGE:       This file contains standard library function for the
//				Cortez VBI Buffer Handler Initialization
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

//#include <math.h>

//#include "LibInc\gm_Global.h"
//#include "LibInc\gm_HostReg.h"
//#include "LibInc\gm_Register.h"
//#include "LibInc\gm_StdLib.h"
//#include "LibInc\gm_Mcu186.h"
//#include "LibInc\gm_Vbi.h"
//#include "LibInc\gm_DecoderModeStable.h"


#include <string.h>
#include "System\all.h"
#include "TestHarness\AppsTest.h"
#include "StdLibInc\gm_Register.h"


#ifdef ISR_MONITOR_R_S_VALUES

#include "StdLibInc\gm_Mcu186.h"

#if (FEATURE_INTERRUPT_SERVICE == REV2)
void interrupt TransfRSvectorPC (void);
void far interrupt gm_MainInputIsrProcess (void);
#endif


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define RS_samples 				1000
#define VM_samples 				(RS_samples>>2) // RS take 2 DWORD, VM 1 BYTE

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

WORD RS_vector[RS_samples];  //<<-------------
BYTE VM_vector[VM_samples];  //<<-------------
BYTE FieldPolarity_vector[VM_samples];
WORD RS_vector_index = 0;
WORD VM_vector_index = 0;

//******************************************************************************
//  L O C A L    F U N C T I O N S
//******************************************************************************

//******************************************************************************
//
// FUNCTION       : void StartRScollect(void)
//
// USAGE          : Issue interrupt and run Film Mode R&S signals monitoring
//                  This appstest will start an interrupt driven by the vertical pulse.
//                  750 samples of the R and S signals will be collected and stored in a vector.
//                  Then when the vector is full, the data will be transferred to the PC
//                  via GProbe in order to be analyzed off-line in the PC.
//                  This is useful for AFM/MADI debugging and global motion analysis used in
//                  the development of adaptive algorithms.
//
// INPUT          : None
//
// OUTPUT         : None
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
void StartRScollect(void)
{
#if (FEATURE_INTERRUPT_SERVICE == REV2)
   gm_InterruptSetFunc(IRQ3, (FPT_ISR)TransfRSvectorPC); //Link interrupt with ISR function name
   INTERRUPT_ENABLE(MAIN_INPUT_IRQ3MASK, IMP_VS_MASK); //Enable interrupt
#endif
   gm_Print("Starting R and S data collection...",0);
}



//******************************************************************************
//
// FUNCTION       : void far TransfRSvectorPC ()  //
//
// USAGE          : The function services the IMP_FLAGLINE interrupt to monitor new
//                  data in the VBI circular buffer.
//
// INPUT          : None
//
// OUTPUT         : None
//
// GLOBALS        : decoderState			(R)
//                  fsc_lock_status_isr		(R/W)
//                  max_fsc					(R/W)
//                  min_fsc					(R/W)
//                  VBI_Data_Ready (R) update to indicate if data in circular buffer
//                  VBI_BufWr_Addr			(R/W)
//
// USED_REGS      : VBI_BUF_WR_ADDR_hi		(R)
//                  VBI_BUF_WR_ADDR_lo		(R)
//                  FSC_LOCK_STATUS_hi		(R)
//                  Notes:
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
//***********************************************************************************************************************************************************
// By: Eduardo Corral. Aug/2004
// The ProcImgGlobalMotion function reads out the AFM R and S values (high and low 16-bit words) for every field. Then it just simply transfers the data vetor
// to GProbe by using the print fucntion
//***********************************************************************************************************************************************************
#if (FEATURE_INTERRUPT_SERVICE == REV2)
void interrupt TransfRSvectorPC (void)  //
{
   WORD W_MainImpStatus = gm_ReadRegWord(MAIN_INPUT_STATUS);
   asm sti; // Allow other interrupt
#endif

   if ((gm_ReadRegWord(MAIN_INPUT_IRQ3MASK) & IMP_VS_MASK) && (W_MainImpStatus & IMP_VS))
   {
      WORD Rhi=0;
      WORD Rlo=0;
      WORD Shi=0;
      WORD Slo=0;
      WORD i=0;
      WORD j=0;

      // Read the R and S values
      Rhi = gm_ReadRegWord(AFM_R_hi);
      Rlo = gm_ReadRegWord(AFM_R_lo);
      Shi = gm_ReadRegWord(AFM_S_hi);
      Slo = gm_ReadRegWord(AFM_S_lo);

      j = 0x7F & gm_ReadRegByte(AFM_22_MOTION);
      // Insert the R and S high and low 16-bit values into the output vector
      RS_vector[RS_vector_index + 0] = Rhi;
      RS_vector[RS_vector_index + 1] = Rlo;
      RS_vector[RS_vector_index + 2] = Shi;
      RS_vector[RS_vector_index + 3] = Slo;

      VM_vector[VM_vector_index] = j;
      FieldPolarity_vector[VM_vector_index] = ((gm_ReadRegWord(MISC_STATUS) & IMP_ODD_STATUS)?1:0);

      //Reset the output vector index in order to insert the next set of 4 16-bit words
      if (RS_vector_index < RS_samples-4)
      {
         RS_vector_index = RS_vector_index + 4;
         VM_vector_index++;
      }
      else
      {
         //Turn off interrupt and send vector to the PC
#if (FEATURE_INTERRUPT_SERVICE == REV2)
         INTERRUPT_DISABLE(MAIN_INPUT_IRQ3MASK, IMP_VS_MASK);
#endif

         // Send the output data vector to the PC via GProbe
         for (i=0, VM_vector_index = 0; VM_vector_index < VM_samples; i+=4, VM_vector_index++)
         {
            gm_Printf("%d %d %d %d %d %d", (WORD)RS_vector[i], (WORD)RS_vector[i+1],
                      (WORD)RS_vector[i+2], (WORD)RS_vector[i+3],
                      VM_vector[VM_vector_index],
                      FieldPolarity_vector[VM_vector_index]);
         }
         gm_Printf("\n ... End of measure ..... \n\n", 0);

         //Reset the output vector index if it's full
         RS_vector_index = 0;
         VM_vector_index = 0;

         //Turn on IRQ3 ISR
#if (FEATURE_INTERRUPT_SERVICE == REV2)
         gm_InterruptSetFunc(IRQ3, (FPT_ISR)gm_MainInputIsrProcess);
         INTERRUPT_ENABLE(MAIN_INPUT_IRQ3MASK, IMP_VS_MASK);
#endif
      }
      gm_WriteRegWord (MAIN_INPUT_STATUS, IMP_VS); //clear status flag
   }

#if (FEATURE_INTERRUPT_SERVICE == REV2)
   asm cli; //to avoid interrupt between next command and iret
   WRITE_PCB_REG(EOI, INT3_VTYPE); // Interrupt exit
#endif
}
#endif //#ifdef ISR_MONITOR_R_S_VALUES
