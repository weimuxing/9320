/*
	$Workfile:   ExtDDRSdram.c  $
	$Revision: 1.6 $
	$Date: 2013/03/21 03:02:11 $
*/

#define __EXT_DDR_SDRAM_C__
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
// MODULE	:	ExtDdrSdram.c
//
// USAGE	:	This module contains tests for external OSD
// 				All code specific to the External Micronas OSD Chip applies
//				only to the EV board platform
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\all.h"
#include "TestHarness\AppsTest.h"
#include "StdLibInc\gm_Mcu186.h"
#include <embedded.h>

#include <mem.h>



//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
//#define _DEBUG

#ifdef _DEBUG
#define  msg(a)             gm_Print(a,0)
#define  msg_i(a,b)         gm_Print(a,b)
#else
#define  msg(a)
#define  msg_i(a,b)
#endif

#define BUFF_SIZE						1024
#define MAX_WORDS 						32			// For Read or Write to memory
#define BURST_SIZE						64			// 64 Byte bursts
#define BURST_SIZE_SHIFT				6

//
// This struct data is based on the WR_SDRAM_ADDR register
//
typedef struct
{
   DWORD	DW_BurstAddr;
   BYTE 	B_WordAddr;
   BYTE 	B_ByteLoc;
} St_MemConfig_t;

//******************************************************************************
//  S T A T I C    F U N C T I O N S
//******************************************************************************

//void ReadExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Check, WORD W_Data);
static void WriteExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data);
static DWORD ConvertSdramAddr(DWORD DW_Addr);
static void TrackBitError (WORD W_Correct, WORD W_Actual, WORD W_WordNum);
static void ReadExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Compare, WORD W_Data);
static void WriteExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data);

//******************************************************************************
//  C O N S T A N T
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************

#ifdef RD_WR_EXT_SDRAM


//******************************************************************************
//
// FUNCTION       : void WriteExtMem(void)
//
// USAGE          : Write data into SDRAM
//                  Param(0) - Dummy because the first argument is of the type "BYTE".
//                  Param(1) - StartAddr (DWORD)
//                  Param(2) - EndAddr (DWORD)
//                  Param(3) - Data to write (WORD)
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
#ifdef USER_EXTENDED_GPROBE_CMD
void WriteExtMem(void)
{
   BYTE B_Dummy = (BYTE) GetParam(0);	// 1st parameter is BYTE
   DWORD DW_StartAddr = (DWORD)GetParam(1);
   DWORD DW_EndAddr = (DWORD)GetParam(2);
   WORD W_Data = (WORD)GetParam(3);

   B_Dummy = B_Dummy;

   msg_i ("Unused param = %d", B_Dummy);
   gm_Printf ("Start Address = 0x%lx", (DWORD) DW_StartAddr);
   gm_Printf ("End Address = 0x%lx", (DWORD) DW_EndAddr);
   msg_i ("Data written = 0x%x", (WORD) W_Data);

   WriteExtSdram(DW_StartAddr, DW_EndAddr, W_Data);

   msg ("Write to external SDRAM complete.");
}

//******************************************************************************
//
// FUNCTION       : void ReadExtMem(void)
//
// USAGE          : Read data from SDRAM
//                  Param(0) - Dummy because the first argument is of the type "BYTE".
//                  Param(1) - StartAddr (DWORD)
//                  Param(2) - EndAddr (DWORD)
//                  Param(3) - Compare (0 = compare with param4)  (1=Output read data to GProbe output window)
//                  Param(4) - Data used for for comparison (WORD)
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
void ReadExtMem(void)
{
   BYTE B_Dummy = (BYTE) GetParam(0);	// 1st parameter is BYTE
   DWORD DW_StartAddr = (DWORD)GetParam(1);
   DWORD DW_EndAddr = (DWORD)GetParam(2);
   BYTE B_Compare = (BYTE) GetParam (3);
   WORD W_Data = (WORD)GetParam(4);

   B_Dummy = B_Dummy;

   msg_i ("Unused param = %d", B_Dummy);
   gm_Printf ("Start Address = 0x%lx", (DWORD) DW_StartAddr);
   gm_Printf ("End Address = 0x%lx", (DWORD) DW_EndAddr);
   if (B_Compare == 0)
      msg_i ("Expected Data (for readback comparison) = 0x%x", (WORD) W_Data);
   else
      msg ("Data piped to GProbe output window");

   ReadExtSdram(DW_StartAddr, DW_EndAddr, B_Compare, W_Data);

   msg ("Read from external SDRAM complete.");
}
#endif // USER_EXTENDED_GPROBE_CMD

//******************************************************************************
//
// FUNCTION       : void ReadExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Compare, WORD W_Data)
//
// USAGE          : Performs a read from external DDR SDRAM with the option to compare
//
// INPUT          : DW_StartAddr - start address
//                  DW_EndAddr - end address
//                  B_Compare
//                  W_Data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void ReadExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Compare, WORD W_Data)
{
   WORD    	Wa_ReadData[MAX_WORDS] = {0}; 	// initialize to 0
   DWORD 		DW_Diff;
   BYTE B_Counter;

   gm_Print("Performing Mem READ...",0);

   if (DW_StartAddr > DW_EndAddr)
   {
      gm_Print ("Start address must be before end address", 0);
      return;
   }

   if (DW_StartAddr & BIT0)
   {
      gm_Print ("Start address cannot be odd. Therefore start address decreased by 1.", 0);
      DW_StartAddr -= 1;
   }
   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      gm_Print ("Read length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can read and print out the values
   // immediately passing in the width and stride values (as the difference between start and end addr)
   //
   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramRead_HA(  ConvertSdramAddr(DW_StartAddr),
                        (WORD) DW_Diff,
                        (WORD) DW_Diff,
                        1, 0,
                        Wa_ReadData);

      DW_Diff = DW_Diff >> 1;	// Using a WORD array (but address locations specified in BYTES)
      //
      // Either check contents for match or simply print out to GProbe
      //
      for (B_Counter=0; B_Counter < DW_Diff; B_Counter++)
      {
         //
         // Compare with value
         //
         if (B_Compare == 0)
         {
            if (Wa_ReadData[B_Counter] != W_Data)
            {
               gm_Print ("******************", 0);
               gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
               gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
               gm_Print("Expected:  0x%x", W_Data);
               gm_Print ("******************", 0);
            }
         }
         //
         // Dump Data out
         //
         else
         {
            gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
         }
      }
   }

   //
   // The read back values will require more than 1 burst (64 bytes)
   //
   else
   {
      BYTE 	B_Offset = (BYTE) (DW_Diff % BURST_SIZE);		// Remaining portion of burst
      DWORD 	DW_Counter;
      DW_Diff    = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write
      //
      // Read 1 burst at a time
      //
      for (DW_Counter = 0; DW_Counter < DW_Diff; DW_Counter++)
      {
         gm_SdramRead_HA(ConvertSdramAddr(DW_StartAddr), BURST_SIZE, BURST_SIZE,1, 0,Wa_ReadData);
         //
         //Either Print out the data read or check if matches data (entered by user)
         //
         for (B_Counter=0; B_Counter < MAX_WORDS; B_Counter++)
         {
            //
            // Compare with value
            //
            if (B_Compare == 0)
            {
               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
               }
            }
            //
            // Dump Data out
            //
            else
            {
               gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
            }
         }
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
      }
      //
      // Read remaining data
      //
      if (B_Offset)
      {
         gm_SdramRead_HA( ConvertSdramAddr(DW_StartAddr), B_Offset, B_Offset,1, 0,Wa_ReadData);
         B_Offset = B_Offset >> 1;		// Since array in WORDS
         //
         //Either Print out the data read or check if matches data (entered by user)
         //
         for (B_Counter=0; B_Counter < B_Offset; B_Counter++)
         {
            //
            // Compare with value
            //
            if (B_Compare == 0)
            {
               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
               }
            }
            //
            // Dump Data out
            //
            else
            {
               gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
            }
         }
      }
   }
   gm_Print("Completed READ from external memory",0);
}

//******************************************************************************
//
// FUNCTION       : void WriteExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data)
//
// USAGE          : Performs a write to external DDR SDRAM with the option to compare
//
// INPUT          : DW_StartAddr - start address
//                  DW_EndAddr - end address
//                  W_Data - data to write (flat data, repeated throughout specified address range)
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
void WriteExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data)
{
   WORD		Wa_WriteData[MAX_WORDS];
   DWORD 		DW_Diff;
   BYTE 		B_Counter;

   gm_Print("Performing Mem WRITE...",0);

   if (DW_StartAddr > DW_EndAddr)
   {
      gm_Print ("Start address must be before end address", 0);
      return;
   }
//	The next if condition was implemented not because of any phisical limitation of writing to an odd address but
//	for syncronizing with the similar memory read function that has this limitation.
   if (DW_StartAddr & BIT0)
   {
      gm_Print ("Start address cannot be odd for syncronising with the similar memory read function. Therefore start address decreased by 1.", 0);
      DW_StartAddr -= 1;
   }
   DW_Diff = DW_EndAddr - DW_StartAddr;

//	The next if condition was implemented not because of any write to memory phisical limitation but
//	for syncronizing with the similar memory read function that has this limitation.

   if ( DW_Diff % 2 )
   {
      gm_Print ("Write length must be within word boundary for syncronizing with the similar memory read function. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }

   //
   // Initialize array with contents provided by user
   //
   for (B_Counter=0; B_Counter < MAX_WORDS; B_Counter++)
   {
      Wa_WriteData[B_Counter]= W_Data;
   }

   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      gm_Print ("Write length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can write immediately passing in the width
   // and stride values (as the difference between start and end addr)
   //
   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramWrite_HA (ConvertSdramAddr(DW_StartAddr), (WORD) DW_Diff,
                        (WORD) DW_Diff, 1, 0, (WORD far *)Wa_WriteData,
                        NULL_PTR, 0, BUFF_SIZE*2);
   }
   //
   // Need to write more than 1 burst
   //
   else
   {
      DWORD DW_Counter;
      BYTE B_Offset = (BYTE) (DW_Diff % BURST_SIZE);	// Remaining portion of burst
      DW_Diff  = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write

      //
      // Write 1 burst at a time
      //
      for (DW_Counter= 0; DW_Counter < DW_Diff; DW_Counter++)
      {
         gm_SdramWrite_HA(ConvertSdramAddr(DW_StartAddr), MAX_WORDS*2, MAX_WORDS*2, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
      }
      //
      // For remaining portion that needs to be written
      //
      if (B_Offset)
      {
         gm_SdramWrite_HA(ConvertSdramAddr(DW_StartAddr), (WORD) B_Offset, (WORD) B_Offset, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
      }
   }
   gm_Print ("Completed WRITE to external memory",0);
}
#endif // RD_WR_EXT_SDRAM

#if 1 //def SMART_ISP	// 20090827 #4, Implement SMART_ISP
#if 1 // Avoid possible shooting noise
#define FSB_WRITE_FIFO_AE_TIMEOUT   0x15
#define FSB_DATA_EXPANDER_RDY_TIMEOUT 0x16
#define FSB_WRITE_XFER_IN_TIMEOUT 0x17
#define SDRAM_BURST_SIZE            32   
#define gmRegW(RegAddr) (*(volatile WORD hostreg*const)(RegAddr))
#define WaitConditionTimeout(condition, timeout, log_code, error, label) \
        if(condition) \
        { \
            gmTimerStartWd(timeout); \
            while(condition) \
            { \
                if(gmTimeOutWd) \
                { \
                    _BX = error; \
                    goto label; \
                } \
            } \
        }

#define WAIT_FSB_WR_ALMOST_EMPTY(label)                                             \
            WaitConditionTimeout(                                                   \
                ((gmRegW(WR_SDRAM_CONTROL) & WR_SDRAM_FIFO_ALMOST_EMPTY) == 0),     \
                gW_TimeoutFsbWr,                                                    \
                FSB_WRITE_FIFO_AE_TIMEOUT,                                          \
                ERR_FSB_WR_SDRAM_FIFO_ALMOST_EMPTY,                                 \
                label)

#define WAIT_FSB_EXPANDER_RDY(label)                                                \
            WaitConditionTimeout(                                                   \
                ((gmRegW(WR_SDRAM_CONTROL) & DATA_EXPANDER_RDY) == 0),              \
                gW_TimeoutFsbWr,                                                    \
                FSB_DATA_EXPANDER_RDY_TIMEOUT,                                      \
                ERR_FSB_DATA_EXPANDER_RDY,                                          \
                label)              

#define WAIT_FSB_WR_XFER(label)                                                     \
            WaitConditionTimeout(                                                   \
                (gmRegW(WR_SDRAM_CONTROL) & WR_XFER_IN_PROGRESS),                   \
                gW_TimeoutFsbWr,                                                    \
                FSB_WRITE_XFER_IN_TIMEOUT,                                          \
                ERR_FSB_WR_XFER_IN_PROGRESS,                                        \
                label)   

#define FARPTR2DWORD(pt) (((DWORD)(FP_SEG(pt)) << B_SegBit2Shift) + FP_OFF(pt))


WORD CheckExpandBufAlloc(WORD far *Wp_ExpData)
{
	DWORD dw = FARPTR2DWORD(Wp_ExpData); 
	if (dw > 0x7FFF)  //Cortez Premium hardware restriction : buffer should be located in first 32K
		return 0;
	else
		return (WORD)dw;
}
                

gmt_RET_STAT huge gm_SdramWriteHaEx1 (DWORD D_Addr, WORD W_Stride, WORD W_Width,
								WORD W_Height, WORD W_Offset, WORD far *Wp_Data,
								WORD far *Wp_ExpData, gmt_DataExpandFactor W_ExpCtrl, WORD W_SizeData)
{
	register WORD _es *Wp_Src = (WORD _es *)Wp_Data;

	if((W_Stride == 0) || (W_Width == 0) || (W_Height == 0) || (W_SizeData == 0) ||
		((Wp_ExpData != 0) && ((W_ExpCtrl & DATA_EXPAND_FACTOR) >= EXPAND_8_TO_8) &&
		(CheckExpandBufAlloc(Wp_ExpData) == 0)))
	{
		return ERR_PARAM;
	}

	//request FSB WRITE FIFO
	gm_RequestFSBWriteFifo();

	//configure FSB WRITE FIFO
	(*gFp_SdramWriteInit)(D_Addr, W_Stride, W_Width, W_Height, W_Offset);

	if(Wp_ExpData == NULL_PTR)
	{
		//instead of:
		// register WORD W_Size = (WORD)((((DWORD)W_SizeData * W_Height) + 1) / 2); //product rounded to the 2
		//will use:
		// _DI = ((W_WidthSrc * W_Height) + 1) / 2; 
		asm 
		{ 
  			mov   ax, W_SizeData
   		mul   W_Height
   		add   ax, 1
   		adc   dx, 0
   		shr   dx, 1
   		rcr   ax, 1  
   	}
		_DX = _AX;
		
		//start burst writing
		gm_SetRegBitsWord(WR_SDRAM_CONTROL, (WORD)WR_XFER_IN_PROGRESS);

		_ES = FP_SEG(Wp_Data);

		for(; _DX >= SDRAM_BURST_SIZE ; _DX -= SDRAM_BURST_SIZE)
		{
			//write 32 words
			for(_CL = SDRAM_BURST_SIZE ; _CL ; _CL--)
				*((WORD hostreg *)WR_SDRAM_DATA) = *Wp_Src++;
			
			WAIT_FSB_WR_ALMOST_EMPTY(ABC);
		}

		//write the rest of words
		for(; _DX ; _DX--)
			*((WORD hostreg *)WR_SDRAM_DATA) = *Wp_Src++;		
	}
	else
	{	
		// Initialize the data expander, access by OCM mode
		(*gFp_DataExpandInit)(Wp_ExpData, W_ExpCtrl & ~DATA_EXPAND_DMA_MODE);

		//start burst writing
		gm_SetRegBitsWord(WR_SDRAM_CONTROL, (WORD)WR_XFER_IN_PROGRESS);

		_ES = FP_SEG(Wp_Data);

		if(W_ExpCtrl & DATA_EXPAND_ROW_ADJUSTMENT)
		{
			_DI = (W_SizeData + 1) / 2;
			_BL = W_SizeData % 2;
			
			for(_DX = W_Height ; _DX ; _DX--)
			{
				for(_CX = _DI; _CX; _CX--)
				{
					*((WORD hostreg *)WR_SDRAM_DATA) = *Wp_Src++;
					WAIT_FSB_EXPANDER_RDY(ABC);
				}

				// if the number of bytes is odd
				// change the current data pointer to the next line start byte
				if(_BL)
				{
					((BYTE *)Wp_Src)--;
				}
			}
			*((WORD hostreg *)WR_SDRAM_DATA) = 0x0000; //WORKAROUND FOR HW BUG
				
		}
		else
		{
			//register WORD W_Size = (WORD)((((DWORD)W_SizeData * W_Height) + 1) / 2);
			asm 
			{ 
  				mov   ax, W_SizeData
   			mul   W_Height
   			add   ax, 1
   			adc   dx, 0
   			shr   dx, 1
   			rcr   ax, 1  
   		}
			for(_CX = _AX; _CX; _CX--)
			{
				*((WORD hostreg *)WR_SDRAM_DATA) = *Wp_Src++;

				WAIT_FSB_EXPANDER_RDY(ABC);
			}
		}
	}

	//wait until writing will be completed
	WAIT_FSB_WR_XFER(ABC);
	_BX = OK;
ABC:
//_terminate_write:
	//stop if there is any incomplete transfer
	gm_ClearRegBitsWord(WR_SDRAM_CONTROL, WR_XFER_IN_PROGRESS);

	// always disable the expander after each transfer
	// Note: Since the Expander is doubled buffer, do not disable the Expander
	// 		 based on teh DATA_EXPANDER_RDY as it may not be emptied even the 
	//		 DATA_EXPANDER_RDY is set.  It is save to disable the Expander when
	//		 the FS is empty
	gm_ClearRegBitsWord(WR_SDRAM_DATA_EXPAND_CONTROL, (WORD)DATA_EXPAND_EN);

	gm_ReleaseFSBWriteFifo();

	return _BX;
}
#endif // shooting noise test code
void ReadExtSdram2Buf (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD far * Wa_ReadData)
{

   DWORD 		DW_Diff;

   if (DW_StartAddr > DW_EndAddr)
   {
      msg_i ("Start address must be before end address", 0);
      return;
   }

   if (DW_StartAddr & BIT0)
   {
      msg_i ("Start address cannot be odd. Therefore start address decreased by 1.", 0);
      DW_StartAddr -= 1;
   }
   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      msg_i ("Read length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can read and print out the values
   // immediately passing in the width and stride values (as the difference between start and end addr)
   //
   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramRead_HA(  ConvertSdramAddr(DW_StartAddr),
                        (WORD) DW_Diff,
                        (WORD) DW_Diff,
                        1, 0,
                        (WORD /*far*/ *)Wa_ReadData);

      DW_Diff = DW_Diff >> 1;	// Using a WORD array (but address locations specified in BYTES)
   }

   //
   // The read back values will require more than 1 burst (64 bytes)
   //
   else
   {
      BYTE 	B_Offset = (BYTE) (DW_Diff % BURST_SIZE);		// Remaining portion of burst
      DWORD 	DW_Counter;
      DW_Diff    = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write
      //
      // Read 1 burst at a time
      //
      for (DW_Counter = 0; DW_Counter < DW_Diff; DW_Counter++)
      {
         gm_SdramRead_HA(ConvertSdramAddr(DW_StartAddr), BURST_SIZE, BURST_SIZE,1, 0,(WORD /*far*/ *)Wa_ReadData);
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
         Wa_ReadData+=(BURST_SIZE/2);
      }
      //
      // Read remaining data
      //
      if (B_Offset)
      {
         gm_SdramRead_HA( ConvertSdramAddr(DW_StartAddr), B_Offset, B_Offset,1, 0,(WORD /*far*/ *)Wa_ReadData);
         B_Offset = B_Offset >> 1;		// Since array in WORDS
      }
   }
//    msg("Completed READ from external memory",0);
}

void WriteBuf2ExtSdram (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD far * Wa_WriteData)
{
   DWORD 		DW_Diff;


   if (DW_StartAddr > DW_EndAddr)
   {
      msg_i("Start address must be before end address", 0);
      return;
   }
//	The next if condition was implemented not because of any phisical limitation of writing to an odd address but
//	for syncronizing with the similar memory read function that has this limitation.
   if (DW_StartAddr & BIT0)
   {
      msg_i("Start address cannot be odd for syncronising with the similar memory read function. Therefore start address decreased by 1.", 0);
      DW_StartAddr -= 1;
   }
   DW_Diff = DW_EndAddr - DW_StartAddr;

//	The next if condition was implemented not because of any write to memory phisical limitation but
//	for syncronizing with the similar memory read function that has this limitation.

   if ( DW_Diff % 2 )
   {
      msg_i("Write length must be within word boundary for syncronizing with the similar memory read function. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }



   //
   // Initialize array with contents provided by user
   //

   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      msg_i("Write length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can write immediately passing in the width
   // and stride values (as the difference between start and end addr)
   //


   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramWriteHaEx1 (ConvertSdramAddr(DW_StartAddr), (WORD) DW_Diff,
                        (WORD) DW_Diff, 1, 0, (WORD far *)Wa_WriteData,
                        NULL_PTR, 0, BUFF_SIZE*2);
   }

   //
   // Need to write more than 1 burst
   //
   else
   {
      DWORD DW_Counter;
      BYTE B_Offset = (BYTE) (DW_Diff % BURST_SIZE);	// Remaining portion of burst
      DW_Diff  = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write

      //
      // Write 1 burst at a time
      //
      for (DW_Counter= 0; DW_Counter < DW_Diff; DW_Counter++)
      {

         gm_SdramWriteHaEx1(ConvertSdramAddr(DW_StartAddr), MAX_WORDS*2, MAX_WORDS*2, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
         Wa_WriteData = Wa_WriteData + MAX_WORDS;
      }
      //
      // For remaining portion that needs to be written
      //
      if (B_Offset)
      {
         gm_SdramWriteHaEx1(ConvertSdramAddr(DW_StartAddr), (WORD) B_Offset, (WORD) B_Offset, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
      }
   }
}

#endif // SMART_ISP

#ifdef PDEV_DEBUG

//
// Tracks number of errors at each bit location
// (32 bit bus but every clock cycles gets 2x32 bits of data from framestore)
//
static WORD Wa_DDR_Charaterization [BURST_SIZE];

//
// Requested by PDEV on Jan 6th, 2004 (for debugging purposes only)
//
//******************************************************************************
//
// FUNCTION       : void TrackSdramBitErrorsRdWr(void)
//
// USAGE          : DDR Characterization Testing (WR and RD)
//                  Param(0) - Dummy because the first argument is of the type "BYTE".
//                  Param(1) - StartAddr (DWORD)
//                  Param(2) - EndAddr (DWORD)
//                  Param(3) - Data to write (WORD)
//                  Param(4) - Compare (0 = compare with param3)  (1=Output read data to GProbe output window)
//                  Param(5) - Number of times to repeat this test
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
void TrackSdramBitErrorsRdWr(void)
{
   BYTE B_Dummy = (BYTE) GetParam(0);	// TODO: fix limitation of BYTE returned for 1st parameter
   DWORD DW_StartAddr = (DWORD)GetParam(1);
   DWORD DW_EndAddr = (DWORD)GetParam(2);
   WORD W_Data = (WORD)GetParam(3);
   BYTE B_Compare = (BYTE)GetParam(4);
   WORD W_Loop = (WORD) GetParam(5);
   WORD W_Count;

   gm_Printf ("Unused 1st Parameter =%d", (BYTE) B_Dummy);
   gm_Printf ("Start Address = 0x%lx", (DWORD) DW_StartAddr);
   gm_Printf ("End Address = 0x%lx", (DWORD) DW_EndAddr);
   gm_Printf ("Value to be written (WORD) = 0x%x", (WORD) W_Data);

   if (B_Compare == 0)
      gm_Print ("Comparing with data.", 0);
   else
      gm_Print ("Simple output to GProbe", 0);

   gm_Printf ("Number of times to loop = %d", (WORD) W_Loop);


   for (W_Count = 0; W_Count < W_Loop; W_Count++)
   {
      gm_Print ("Beginning Iteration #%d", W_Count);
      WriteExtSdramToggle(DW_StartAddr, DW_EndAddr, W_Data);
      ReadExtSdramToggle(DW_StartAddr, DW_EndAddr, B_Compare, W_Data);
   }
   gm_Print ("*************",0);

   for (W_Count=0; W_Count < BURST_SIZE; W_Count++)
   {
      gm_Printf ("Bit #%d has %d errors", W_Count, Wa_DDR_Charaterization[W_Count]);
   }
   //
   // Now reset contents of array
   //
   for (W_Count=0; W_Count < BURST_SIZE; W_Count++)
   {
      Wa_DDR_Charaterization[W_Count] = 0;
   }
}

//
// Requested by PDEV on Jan 6th, 2004 (for debugging purposes only)
//
//******************************************************************************
//
// FUNCTION       : void TrackSdramBitErrorsRdOnly(void)
//
// USAGE          : DDR Characterization Testing (RD only)
//                  Param(0) - Dummy because the first argument is of the type "BYTE".
//                  Param(1) - StartAddr (DWORD)
//                  Param(2) - EndAddr (DWORD)
//                  Param(3) - Data to compare with (WORD)
//                  Param(4) - Compare (0 = compare with param3)  (1=Output read data to GProbe output window)
//                  Param(5) - Number of times to repeat this test
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
void TrackSdramBitErrorsRdOnly(void)
{
   BYTE B_Dummy = (BYTE) GetParam(0);	// TODO: fix limitation of BYTE returned for 1st parameter
   DWORD DW_StartAddr = (DWORD)GetParam(1);
   DWORD DW_EndAddr = (DWORD)GetParam(2);
   WORD W_Data = (WORD)GetParam(3);
   BYTE B_Compare = (BYTE)GetParam(4);
   WORD W_Loop = (WORD) GetParam(5);
   WORD W_Count;

   gm_Printf ("Unused 1st Parameter =%d", (BYTE) B_Dummy);
   gm_Printf ("Start Address = 0x%lx", (DWORD) DW_StartAddr);
   gm_Printf ("End Address = 0x%lx", (DWORD) DW_EndAddr);
   gm_Printf ("Value to compare against (WORD) = 0x%x", (WORD) W_Data);

   if (B_Compare == 0)
      gm_Print ("Comparing with data.", 0);
   else
      gm_Print ("Simple output to GProbe", 0);

   gm_Printf ("Number of times to loop = %d", (WORD) W_Loop);


   for (W_Count = 0; W_Count < W_Loop; W_Count++)
   {
      gm_Print ("Beginning Iteration #%d", W_Count);
      ReadExtSdramToggle(DW_StartAddr, DW_EndAddr, B_Compare, W_Data);
   }
   gm_Print ("*************",0);

   for (W_Count=0; W_Count < BURST_SIZE; W_Count++)
   {
      gm_Printf ("Bit #%d has %d errors", W_Count, Wa_DDR_Charaterization[W_Count]);
   }
   //
   // Now reset contents of array
   //
   for (W_Count=0; W_Count < BURST_SIZE; W_Count++)
   {
      Wa_DDR_Charaterization[W_Count] = 0;
   }
}

//******************************************************************************
//
// FUNCTION       : void TrackBitError (WORD W_Correct, WORD W_Actual, WORD W_WordNum)
//
// USAGE          : Tracks which bits are not matching
//
// INPUT          : W_Correct - expected value of data
//                  W_Actual - actual data read back
//                  W_WordNum - indicates which word is being checked against (0-3) since 64 bits of data
//                     retrieved per cycle
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void TrackBitError (WORD W_Correct, WORD W_Actual, WORD W_WordNum)
{
   WORD W_Error = 0;
   BYTE B_Shift = 0;
   BYTE B_Offset;
   W_Error = W_Correct ^ W_Actual;		// xor to find non-matching bit locations

   //
   // Determine offset based on which word we are checking (0-3)
   //
   switch (W_WordNum)
   {
      case 0:
         B_Offset = 0;
         break;

      case 1:
         B_Offset = 16;
         break;

      case 2:
         B_Offset = 32;
         break;

      case 3:
         B_Offset = 48;
         break;

      default:
         B_Offset = 0;
         break;
   }

   //
   // Find all 1's (non-matching bits) and continue to right shift xor'd data
   //
   for (B_Shift = 0; B_Shift < 16; B_Shift++)
   {
      if (W_Error & BIT0)
      {
         gm_Print ("Error at bit %d", B_Offset+B_Shift);
         Wa_DDR_Charaterization[B_Offset + B_Shift]++; 	// Increment counter for bit error
      }
      W_Error >>= 1;		// Shift out and check next bit for error
   }
}

//******************************************************************************
//
// FUNCTION       : static void ReadExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Compare, WORD W_Data)
//
// USAGE          : Performs a read with the option to compare
//                  (Note: width * Height should be a multiple of 16)
//
// INPUT          : DW_StartAddr
//                  DW_EndAddr
//                  B_Compare
//                  W_Data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void ReadExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, BYTE B_Compare, WORD W_Data)
{
   WORD    	Wa_ReadData[MAX_WORDS] = {0}; 	// initialize to 0
   DWORD 		DW_Diff;
   BYTE B_Counter;

   gm_Print("Performing Mem READ...",0);

   if (DW_StartAddr > DW_EndAddr)
   {
      gm_Print ("Start address must be before end address", 0);
      return;
   }

   if (DW_StartAddr & BIT0)
   {
      gm_Print ("Start address cannot be odd. Therefore start address decreased by 1.", 0);
      DW_StartAddr -= 1;
   }
   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      gm_Print ("Read length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can read and print out the values
   // immediately passing in the width and stride values (as the difference between start and end addr)
   //
   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramRead_HA(  ConvertSdramAddr(DW_StartAddr),
                        (WORD) DW_Diff,
                        (WORD) DW_Diff,
                        1, 0,
                        Wa_ReadData);

      DW_Diff = DW_Diff >> 1;	// Using a WORD array (but address locations specified in BYTES)
      //
      // Either check contents for match or simply print out to GProbe
      //
      for (B_Counter=0; B_Counter < DW_Diff; B_Counter++)
      {
         //
         // Compare with value
         //
         if (B_Compare == 0)
         {
            //
            // Should optimize later (this was requested very quickly for h/w validation)
            // The data is sent in as
            // (1) Data
            // (2) Data
            // (3) Data (toggled)
            // (4) Data (toggled)
            // repeatedly (stop condition determined by end address specified)
            //
            if (Wa_ReadData[B_Counter] != W_Data)
            {
               gm_Print ("******************", 0);
               gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
               gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
               gm_Print("Expected:  0x%x", W_Data);
               gm_Print ("******************", 0);
               gm_Print ("Word 0", 0);
               TrackBitError(W_Data, Wa_ReadData[B_Counter], 0);
            }
            B_Counter++;

            if (Wa_ReadData[B_Counter] != W_Data)
            {
               gm_Print ("******************", 0);
               gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
               gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
               gm_Print("Expected:  0x%x", W_Data);
               gm_Print ("******************", 0);
               gm_Print ("Word 1", 0);
               TrackBitError(W_Data, Wa_ReadData[B_Counter], 1);
            }
            B_Counter++;
            W_Data = ~W_Data;				// Check toggled data

            if (Wa_ReadData[B_Counter] != W_Data)
            {
               gm_Print ("******************", 0);
               gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
               gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
               gm_Print("Expected:  0x%x", W_Data);
               gm_Print ("******************", 0);
               gm_Print ("Word 2", 0);
               TrackBitError(W_Data, Wa_ReadData[B_Counter], 2);
            }
            B_Counter++;

            if (Wa_ReadData[B_Counter] != W_Data)
            {
               gm_Print ("******************", 0);
               gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
               gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
               gm_Print("Expected:  0x%x", W_Data);
               gm_Print ("******************", 0);
               gm_Print ("Word 3", 0);
               TrackBitError(W_Data, Wa_ReadData[B_Counter], 3);
            }
            W_Data = ~W_Data;			// Restore to original data
         }
         //
         // Dump Data out
         //
         else
         {
            gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
         }
      }
   }

   //
   // The read back values will require more than 1 burst (64 bytes)
   //
   else
   {
      BYTE 	B_Offset = (BYTE) (DW_Diff % BURST_SIZE);		// Remaining portion of burst
      DWORD 	DW_Counter;
      DW_Diff    = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write
      //
      // Read 1 burst at a time
      //
      for (DW_Counter = 0; DW_Counter < DW_Diff; DW_Counter++)
      {
         gm_SdramRead_HA(ConvertSdramAddr(DW_StartAddr), BURST_SIZE, BURST_SIZE,1, 0,Wa_ReadData);
         //
         //Either Print out the data read or check if matches data (entered by user)
         //
         for (B_Counter=0; B_Counter < MAX_WORDS; B_Counter++)
         {
            //
            // Compare with value
            //
            if (B_Compare == 0)
            {
               //
               // Should optimize later (this was requested very quickly for h/w validation)
               // The data is sent in as
               // (1) Data
               // (2) Data
               // (3) Data (toggled)
               // (4) Data (toggled)
               // repeatedly (stop condition determined by end address specified)
               //
               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 0", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 0);
               }
               B_Counter++;

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 1", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 1);
               }
               B_Counter++;
               W_Data = ~W_Data;				// Check toggled data

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 2", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 2);
               }
               B_Counter++;

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at:  0x%lx", (DWORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Read back: 0x%x ", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected:  0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 3", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 3);
               }
               W_Data = ~W_Data;			// Restore to original data
            }
            //
            // Dump Data out
            //
            else
            {
               gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
            }
         }
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
      }
      //
      // Read remaining data
      //
      if (B_Offset)
      {
         gm_SdramRead_HA( ConvertSdramAddr(DW_StartAddr), B_Offset, B_Offset,1, 0,Wa_ReadData);
         B_Offset = B_Offset >> 1;		// Since array in WORDS
         //
         //Either Print out the data read or check if matches data (entered by user)
         //
         for (B_Counter=0; B_Counter < B_Offset; B_Counter++)
         {
            //
            // Compare with value
            //
            if (B_Compare == 0)
            {
               //
               // Should optimize later (this was requested very quickly for h/w validation)
               // The data is sent in as
               // (1) Data
               // (2) Data
               // (3) Data (toggled)
               // (4) Data (toggled)
               // repeatedly (stop condition determined by end address specified)
               //
               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at 0x%x%x", (WORD) (DW_StartAddr+B_Counter+1)>>16,
                             (WORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Value Read was 0x%x", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected Value 0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 0", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 0);
               }
               B_Counter++;

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at 0x%x%x", (WORD) (DW_StartAddr+B_Counter+1)>>16,
                             (WORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Value Read was 0x%x", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected Value 0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 1", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 1);
               }
               B_Counter++;
               W_Data = ~W_Data;				// Check toggled data

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at 0x%x%x", (WORD) (DW_StartAddr+B_Counter+1)>>16,
                             (WORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Value Read was 0x%x", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected Value 0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 2", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 2);
               }
               B_Counter++;

               if (Wa_ReadData[B_Counter] != W_Data)
               {
                  gm_Print ("******************", 0);
                  gm_Printf ("Error at 0x%x%x", (WORD) (DW_StartAddr+B_Counter+1)>>16,
                             (WORD) (DW_StartAddr + (B_Counter << 1)));
                  gm_Print("Value Read was 0x%x", (WORD) Wa_ReadData[B_Counter]);
                  gm_Print("Expected Value 0x%x", W_Data);
                  gm_Print ("******************", 0);
                  gm_Print ("Word 3", 0);
                  TrackBitError(W_Data, Wa_ReadData[B_Counter], 3);
               }
               W_Data = ~W_Data;			// Restore to original data

            }
            //
            // Dump Data out
            //
            else
            {
               gm_Printf ("0x%lx = 0x%x", (DWORD) (DW_StartAddr + (B_Counter << 1)),  Wa_ReadData[B_Counter]);
            }
         }
      }
   }
   gm_Print("Completed READ from external memory",0);
}

//******************************************************************************
//
// FUNCTION       : void WriteExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data)
//
// USAGE          : Performs Write to external memory
//
// INPUT          : DW_StartAddr, DW_EndAddr,
//                  DW_EndAddr
//                  W_Data
//
// OUTPUT         : None
//
// GLOBALS        : None
//
// USED_REGS      :
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void WriteExtSdramToggle (DWORD DW_StartAddr, DWORD DW_EndAddr, WORD W_Data)
{
   WORD		Wa_WriteData[MAX_WORDS];
   DWORD 		DW_Diff;
   BYTE 		B_Counter;

   gm_Print("Performing Mem WRITE...",0);

   if (DW_StartAddr > DW_EndAddr)
   {
      gm_Print ("Start address must be before end address", 0);
      return;
   }
   //
   // Initialize array with contents provided by user (will place data then toggled data next)
   //
   for (B_Counter=0; B_Counter < MAX_WORDS; B_Counter++)
   {
      Wa_WriteData[B_Counter++] = W_Data;
      Wa_WriteData[B_Counter++]   = W_Data;
      Wa_WriteData[B_Counter++] = ~W_Data;
      Wa_WriteData[B_Counter]     = ~W_Data;
   }

   DW_Diff = DW_EndAddr - DW_StartAddr;

   if ( DW_Diff % 2 )
   {
      gm_Print ("Write length must be within word boundary. Therefore end address increased by 1", 0);
      DW_EndAddr++;
      DW_Diff++;
   }
   //
   // If end address is less than 1 burst (i.e. 64 bytes), we can write immediately passing in the width
   // and stride values (as the difference between start and end addr)
   //
   if (DW_Diff <= BURST_SIZE)
   {
      gm_SdramWrite_HA(ConvertSdramAddr(DW_StartAddr), (WORD) DW_Diff, (WORD) DW_Diff, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
   }
   //
   // Need to write more than 1 burst
   //
   else
   {
      DWORD DW_Counter;
      BYTE B_Offset = (BYTE) (DW_Diff % BURST_SIZE);	// Remaining portion of burst
      DW_Diff  = DW_Diff >> BURST_SIZE_SHIFT;			// Number of bursts to write

      //
      // Write 1 burst at a time
      //
      for (DW_Counter= 0; DW_Counter < DW_Diff; DW_Counter++)
      {
         gm_SdramWrite_HA(ConvertSdramAddr(DW_StartAddr), MAX_WORDS*2, MAX_WORDS*2, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
         DW_StartAddr = DW_StartAddr + BURST_SIZE;
      }
      //
      // For remaining portion that needs to be written
      //
      if (B_Offset)
      {
         gm_SdramWrite_HA(ConvertSdramAddr(DW_StartAddr), (WORD) B_Offset, (WORD) B_Offset, 1, 0, (WORD far *)Wa_WriteData, NULL_PTR, 0, BUFF_SIZE*2);
      }
   }
   gm_Print ("Completed WRITE to external memory",0);
}

#endif //#ifdef PDEV_DEBUG

#if 1 //defined(RD_WR_EXT_SDRAM) || defined(PDEV_DEBUG)|| defined(SMART_ISP)
//******************************************************************************
//
// FUNCTION       : DWORD ConvertSdramAddr(DWORD DW_Addr)
//
// USAGE          : Helper function to convert the address specifed to SDRAM address prior
//                  to calling driver function
//
// INPUT          : DW_Addr - start address
//
// OUTPUT         : DWORD - Address converted for read/write by memory controller
//
// GLOBALS        : None
//
// USED_REGS      : None
//
// PRE_CONDITION  : None
//
// POST_CONDITION : None
//
//******************************************************************************
DWORD ConvertSdramAddr(DWORD DW_Addr)
#if 1
{
   St_MemConfig_t St_MemConfig;
   //
   // Simplest case: Address is 0
   //
   if (DW_Addr == 0)
   {
      St_MemConfig.DW_BurstAddr 	= 0;
      St_MemConfig.B_WordAddr 	= 0;
      St_MemConfig.B_ByteLoc 	= 0;
   }
   //
   // If specified address divides perfectly into a burst, no offsets at all
   //
   else if ((DW_Addr >= BURST_SIZE)  &&  (DW_Addr % BURST_SIZE == 0))
   {
      DW_Addr = DW_Addr >> BURST_SIZE_SHIFT;
      St_MemConfig.DW_BurstAddr = DW_Addr;			// In first burst because address less than burst size
      St_MemConfig.B_WordAddr 	= 0;
      St_MemConfig.B_ByteLoc 	= 0;
   }

   //
   // If smaller than 1 burst
   //
   else if (DW_Addr < BURST_SIZE)
   {
      BYTE B_Temp = 0;
      BYTE B_Offset = 0;

      St_MemConfig.DW_BurstAddr = 0;			// In first burst because address less than burst size
      B_Temp = (BYTE) (DW_Addr-1) >> 2;		// Word address within burst (div by 4)
      St_MemConfig.B_WordAddr = B_Temp;

      B_Offset = (BYTE) (DW_Addr % 4);
      St_MemConfig.B_ByteLoc = B_Offset;
   }

   //
   // Greater than 1 burst
   //
   else
   {
      DWORD DW_Temp = 0;
      BYTE B_Temp = 0;
      BYTE B_Offset = 0;

      DW_Temp = DW_Addr >> BURST_SIZE_SHIFT;
      St_MemConfig.DW_BurstAddr = DW_Temp;

      // Word address within burst (div by 4)
//		B_Temp = (BYTE) ( ((DW_Addr - (DW_Temp << BURST_SIZE_SHIFT))-1) >> 2 );
      B_Temp = (BYTE) ( ((DW_Addr - (DW_Temp << BURST_SIZE_SHIFT))) >> 2 );
      St_MemConfig.B_WordAddr = B_Temp;

      B_Offset = (BYTE) (DW_Addr % 4);
      St_MemConfig.B_ByteLoc = B_Offset;
   }
//	msg_i ("AddrHi = %x", (WORD) ((St_MemConfig.DW_BurstAddr >> 16) & 0xFF));
//	msg_i ("AddrLo = %x", (WORD) ((St_MemConfig.DW_BurstAddr) & 0xFF));
//	msg_i ("WordAddr = %x", (BYTE) (St_MemConfig.B_WordAddr));
//	msg_i ("ByteLoc = %x", (BYTE) (St_MemConfig.B_ByteLoc));

   DW_Addr = (St_MemConfig.B_ByteLoc);
   DW_Addr = DW_Addr     | (St_MemConfig.B_WordAddr << 2);
   DW_Addr = DW_Addr     | (St_MemConfig.DW_BurstAddr << 6);
   return (DW_Addr);
}
#else
{
   St_MemConfig_t St_MemConfig;
   //
   // Simplest case: Address is 0
   //
   if (DW_Addr == 0)
   {
      St_MemConfig.DW_BurstAddr 	= 0;
      St_MemConfig.B_WordAddr 	= 0;
      St_MemConfig.B_ByteLoc 	= 0;
   }
   //
   // If specified address divides perfectly into a burst, no offsets at all
   //
   else if ((DW_Addr >= BURST_SIZE)  &&  (DW_Addr % BURST_SIZE == 0))
   {
      DW_Addr = DW_Addr >> BURST_SIZE_SHIFT;
      St_MemConfig.DW_BurstAddr = DW_Addr;			// In first burst because address less than burst size
      St_MemConfig.B_WordAddr 	= 0;
      St_MemConfig.B_ByteLoc 	= 0;
   }

   //
   // If smaller than 1 burst
   //
   else if (DW_Addr < BURST_SIZE)
   {
      BYTE B_Temp = 0;
      BYTE B_Offset = 0;

      St_MemConfig.DW_BurstAddr = 0;			// In first burst because address less than burst size
      B_Temp = (BYTE) (DW_Addr-1) >> 2;		// Word address within burst (div by 4)
      St_MemConfig.B_WordAddr = B_Temp;

      B_Offset = (BYTE) (DW_Addr % 4);
      St_MemConfig.B_ByteLoc = B_Offset;
   }

   //
   // Greater than 1 burst
   //
   else
   {
      DWORD DW_Temp = 0;
      BYTE B_Temp = 0;
      BYTE B_Offset = 0;

      DW_Temp = DW_Addr >> BURST_SIZE_SHIFT;
      St_MemConfig.DW_BurstAddr = DW_Temp;

      // Word address within burst (div by 4)
      B_Temp = (BYTE) ( ((DW_Addr - (DW_Temp << BURST_SIZE_SHIFT))-1) >> 2 );
      St_MemConfig.B_WordAddr = B_Temp;

      B_Offset = (BYTE) (DW_Addr % 4);
      St_MemConfig.B_ByteLoc = B_Offset;
   }
//	msg_i ("AddrHi = %x", (WORD) ((St_MemConfig.DW_BurstAddr >> 16) & 0xFF));
//	msg_i ("AddrLo = %x", (WORD) ((St_MemConfig.DW_BurstAddr) & 0xFF));
//	msg_i ("WordAddr = %x", (BYTE) (St_MemConfig.B_WordAddr));
//	msg_i ("ByteLoc = %x", (BYTE) (St_MemConfig.B_ByteLoc));

   DW_Addr = (St_MemConfig.B_ByteLoc);
   DW_Addr = DW_Addr     | (St_MemConfig.B_WordAddr << 2);
   DW_Addr = DW_Addr     | (St_MemConfig.DW_BurstAddr << 6);
   return (DW_Addr);
}
#endif

#endif // #if defined(RD_WR_EXT_SDRAM) || defined(PDEV_DEBUG)
//*********************************  END  **************************************

