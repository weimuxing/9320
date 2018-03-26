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
// MODULE:		Memory.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "System\All.h"
#include "StdLibInc\gm_Register.h"


//******************************************************************************
//  D E B U G
//******************************************************************************

//#define DEBUG_MSG
#ifdef DEBUG_MSG
#define msg(x,y) gm_Print(x,y)
#else
#define msg(x,y)
#endif


//******************************************************************************
//  D E F I N E S
//******************************************************************************

#define MIN_SIZE(X,Y) ((X)<(Y)?(X):(Y))
#define MEMORY_ALLOCATION_SAFETY_COUNT	10

//******************************************************************************
//  S T A T I C S
//******************************************************************************

static void MemorySetCapturePrefs(gmt_PHY_CH B_Channel);
static void MemoryAllocationStatic(gmt_PHY_CH B_Channel);
static void MemoryAllocationDynamic(gmt_PHY_CH B_Channel);
static BOOL IsHardwareLimitBpp(gmt_PHY_CH B_Channel);

//******************************************************************************
//  G L O B A L    D E F I N E S
//******************************************************************************
gmt_CUSTOM_BUFF_TYPE 	gB_MemoryCustomBufferAllocated;	//W keep track of which custom Memory Buffer types is allocated

//******************************************************************************
//  E X T E R N S
//******************************************************************************

extern gmt_BUFF_TYPE gb_MemoryStaticBuffers;
extern 	ROM DWORD gmc_MemoryPalAutoInstallSize;
#ifdef LCDTV_PAL
extern BYTE gB_PAL_AutoInstall_MainChannelBuffer_TempReduction;
extern DWORD gD_TunerAutoInstallSDRAMStartAddr;
#endif

//******************************************************************************
//  C O N S T A N T S
//******************************************************************************

//
// Static allocation parameters:	These are used only when the static memory allocation
//									method is chosen.  The parameters specify the maximum
//									input sizes allowed.  This information is used at startup
//									to calculate how much memory is required to be statically
//									allocated for the MAIN and PIP channels.
//
ROM gmt_MemoryAllocateParams gmc_MemoryAllocateParams_Unlimited =
{
   #if(IS_PROJ_4K2K())
	3840,
   #else
   2560, // 1920,	// W_MainMaxGraphicsWidth
   #endif
   2160, //1600, // 1200,	// W_MainMaxGraphicsHeight
#if (defined (ENABLE_8BIT_MEMORY))
   24,		// B_MainMaxGraphicBpp
#else
   30,		// B_MainMaxGraphicBpp
#endif
   3,		// B_MainMaxGraphicNumBuffers

   1920,	// W_MainMaxVideoProgresiveWidth
   1080,	// W_MainMaxVideoProgresiveHeight
#if (defined (ENABLE_8BIT_MEMORY))
   16,		// B_MainMaxVideoProgresiveBpp
#else
   20,		// B_MainMaxVideoProgresiveBpp
#endif
   3,		// B_MainMaxVideoProgresiveNumBuffers

   1920,	// W_MainMaxVideoInterlacedWidth
   540,	// W_MainMaxVideoInterlacedHeight
#if (defined (ENABLE_8BIT_MEMORY))
   16,		// B_MainMaxVideoProgresiveBpp
#else
   20,		// B_MainMaxVideoInterlacedBpp
#endif
   5,		// B_MainMaxVideoInterlacedNumBuffers

   2560, // 1920,	// W_PipMaxGraphicsWidth
   2160, //1600, // 1200,	// W_PipMaxGraphicsHeight
#if (defined (ENABLE_8BIT_MEMORY))
   24,		// B_PipMaxGraphicBpp
#else
   30,		// B_PipMaxGraphicBpp
#endif
   3,		// B_PipMaxGraphicNumBuffers


   1920,	// W_PipMaxVideoWidth
   1080,	// W_PipMaxVideoHeight
   20,		// B_PipMaxVideoBpp
   3,		// B_PipMaxVideoNumBuffers;
};

ROM gmt_MemoryAllocateParams gmc_MemoryAllocateParams_WUXGA_Panel =
{
   1600,	// W_MainMaxGraphicsWidth
   1200,	// W_MainMaxGraphicsHeight
   30,		// B_MainMaxGraphicBpp
   2,		// B_MainMaxGraphicNumBuffers

   1920,	// W_MainMaxVideoProgresiveWidth
   1080,	// W_MainMaxVideoProgresiveHeight
   20,		// B_MainMaxVideoProgresiveBpp
   3,		// B_MainMaxVideoProgresiveNumBuffers

   1920,	// W_MainMaxVideoInterlacedWidth
   540,	// W_MainMaxVideoInterlacedHeight
   20,		// B_MainMaxVideoInterlacedBpp
   5,		// B_MainMaxVideoInterlacedNumBuffers

   1600,	// W_PipMaxGraphicsWidth
   1200,	// W_PipMaxGraphicsHeight
   24,		// B_PipMaxGraphicBpp
   2,		// B_PipMaxGraphicNumBuffers

   1920,	// W_PipMaxVideoWidth
   1080,	// W_PipMaxVideoHeight
   20,		// B_PipMaxVideoBpp
   2,		// B_PipMaxVideoNumBuffers;
};

ROM gmt_MemoryAllocateParams gmc_MemoryAllocateParams_1080P_Panel =
{
   1600,	// W_MainMaxGraphicsWidth
   1200,	// W_MainMaxGraphicsHeight
   24,		// B_MainMaxGraphicBpp
   3,		// B_MainMaxGraphicNumBuffers

   1920,	// W_MainMaxVideoProgresiveWidth
   1080,	// W_MainMaxVideoProgresiveHeight
   20,		// B_MainMaxVideoProgresiveBpp
   3,		// B_MainMaxVideoProgresiveNumBuffers

   1920,	// W_MainMaxVideoInterlacedWidth
   540,	// W_MainMaxVideoInterlacedHeight
   20,		// B_MainMaxVideoInterlacedBpp
   4,		// B_MainMaxVideoInterlacedNumBuffers

   1600,	// W_PipMaxGraphicsWidth
   1200,	// W_PipMaxGraphicsHeight
   30,		// B_PipMaxGraphicBpp
   2,		// B_PipMaxGraphicNumBuffers

   1920,	// W_PipMaxVideoWidth
   1080,	// W_PipMaxVideoHeight
   20,		// B_PipMaxVideoBpp
   2,		// B_PipMaxVideoNumBuffers;
};

ROM gmt_MemoryAllocateParams gmc_MemoryAllocateParams_UXGA_Panel =
{
   1600,	// W_MainMaxGraphicsWidth
   1200,	// W_MainMaxGraphicsHeight
   30,		// B_MainMaxGraphicBpp
   2,		// B_MainMaxGraphicNumBuffers

   1920,	// W_MainMaxVideoProgresiveWidth
   1080,	// W_MainMaxVideoProgresiveHeight
   20,		// B_MainMaxVideoProgresiveBpp
   3,		// B_MainMaxVideoProgresiveNumBuffers

   1920,	// W_MainMaxVideoInterlacedWidth
   540,	// W_MainMaxVideoInterlacedHeight
   20,		// B_MainMaxVideoInterlacedBpp
   5,		// B_MainMaxVideoInterlacedNumBuffers

   1600,	// W_PipMaxGraphicsWidth
   1200,	// W_PipMaxGraphicsHeight
   30,		// B_PipMaxGraphicBpp
   2,		// B_PipMaxGraphicNumBuffers

   1920,	// W_PipMaxVideoWidth
   1080,	// W_PipMaxVideoHeight
   20,		// B_PipMaxVideoBpp
   3,		// B_PipMaxVideoNumBuffers;
};


//*****************************************************************************
//  C O D E
//*****************************************************************************

//******************************************************************************
//
// FUNCTION		:	static BOOL IsHardwareLimitBpp(gmt_PHY_CH B_Channel)
//
// USAGE		:	Encapsulates hardware limitation scenarios that limit higher Bpp to be
//					used.
//
// INPUT		:	B_Channel - CH_A or CH_B
//
// OUTPUT		:	Returns TRUE if limitation exists; otherwise, returns FALSE
//
// GLOBALS		:	gmvw_InputHTotal[]		(R)
//					gmvw_InputHFreq[]		(R)
//					gmvw_InputVTotal[]		(R)
//					gmvw_InputVFreq[]		(R)
//					gmvw_InputHActive[]		(R)
//					gmvw_InputVActive[]		(R)
//
// USED_REGS	:	None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BOOL IsHardwareLimitBpp(gmt_PHY_CH B_Channel)
{
   BOOL Bt_Result = FALSE;

	// To Do 20120423, reserved for bandwidth condition.
   B_Channel = B_Channel; 

   return Bt_Result;
}

//******************************************************************************
//
// FUNCTION		:	static void MemorySetCapturePrefs(gmt_PHY_CH B_Channel)
//
// USAGE		:	Sets the prefered memory capture settings for the current
//					mode.  This includes which memory buffers are needed to
//					be allocated, how many bits per pixel will be captured
//					to memory, and how many buffers will be used.
//
// INPUT		:	B_Channel - CH_A or CH_B
//
// OUTPUT		:	None
//
// GLOBALS		:	gmvb_MemoryBpp			(RW)
//					gmvb_MemoryNumBuffers	(RW)
//					gmvb_MemoryBufferTypes	(RW)
//
// USED_REGS	:	None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void MemorySetCapturePrefs(gmt_PHY_CH B_Channel)
{
   // Choose the preferred number of frame buffers, and buffer types
   // This includes which memory buffers are needed to be allocated, how many bits per pixel will
   // be captured to memory, and how many buffers will be used.
   if (B_Channel == CH_A)
   {
      if ((gm_IsGraphics(B_Channel)&&(!(gm_IsInterlaced(B_Channel))))
            || !(gm_IsInterlaced(B_Channel))
            || (gmvw_InputVActiveOrg[B_Channel] > PanelHeight))
      {
         // For progressive modes the desired number of frame buffers is 3.
         // For interlaced modes that have a vertical shrink we use 1 field processing, hence
         // only 3 frame buffers are required.
#if	1 // 20080717 #2, ... Full 10 bits without reduce for Mars
         #if 1
         if(IsGameModeApplicable() && (UserPrefGameMode == 1))
         #else
         if (gm_IsGraphics(B_Channel) && (UserPrefGameMode == 1) && 
            (abs(stInput[B_Channel].Vfrq - 600) < 10))
         #endif
         {
            gmvb_MemoryNumBuffers[B_Channel] = 1; // 3;
         }
         else
            gmvb_MemoryNumBuffers[B_Channel] = 3; // 2; 3 for enable progressive TNR
#else
         gmvb_MemoryNumBuffers[B_Channel] = 3;
#endif
         gmvb_MemoryBufferTypes[B_Channel] = 0;
      }
      else
      {
         // For interlaced modes through CH_A the desired number of frame buffers is 5.
         gmvb_MemoryNumBuffers[B_Channel] = 5;
         //gmvb_MemoryBufferTypes[B_Channel] = (BUF_MOTION_VECTORS | BUF_CCS);
         gmvb_MemoryBufferTypes[B_Channel] = (BUF_MOTION_VECTORS);
      }

   }
   else
   {
      // For all modes through CH_B the desired number of frame buffers is 3.
      #if 1
      if(IsGameModeApplicable() && (UserPrefGameMode == 1))
      #else
      if (gm_IsGraphics(B_Channel) && (UserPrefGameMode == 1) &&
         (abs(stInput[B_Channel].Vfrq - 600) < 10))
      #endif
      {
         gmvb_MemoryNumBuffers[B_Channel] = 1; // 3;
      }
      else
      {
			#ifdef VIDEO_CAPTURE  // Reduce PIP memory size for video capture, if PIP is graphic input.
         if (gm_IsGraphics(B_Channel))
            gmvb_MemoryNumBuffers[B_Channel] = 1;
         else
			#endif
			gmvb_MemoryNumBuffers[B_Channel] = 3;	 // 2; // 20090217 #1, resolve frame tear
      }
      
      gmvb_MemoryBufferTypes[B_Channel] = 0;
   }

   {
      // Determine if the input is routed through a decoder
      BYTE B_IsDecoderRouting = FALSE;

      // If the input is routed (or will be routed) through the decoder, allocate a buffer for it.
      if (B_IsDecoderRouting)
      {
         gmvb_MemoryBufferTypes[B_Channel] |= BUF_DECODER;
         //Remove flag for decoder of other channel if decoder is used for this channel
         gmvb_MemoryBufferTypes[gm_OtherChannel(B_Channel)] &= (~BUF_DECODER);
      }
   }

   // Check if any memory buffers are already setup for the other channel and do not need to be setup again.
   gmvb_MemoryBufferTypes[B_Channel] &= ~gmvb_MemoryBufferTypes[gm_OtherChannel(B_Channel)];

   // Choose the desired BPP
	#if (FEATURE_PIP_RGB_COLORSPACE == ENABLE)
   if (gm_GetChannelInputAttr(B_Channel, gmd_COLOR_SPACE) == gmd_RGB)
   {
		#if (defined (ENABLE_8BIT_MEMORY))
      gmvb_MemoryBpp[B_Channel] = 24;
		#else // (defined (ENABLE_8BIT_MEMORY))
      if (IsHardwareLimitBpp(B_Channel))
         gmvb_MemoryBpp[B_Channel] = 24;
      else
		{
			if(B_Channel == gmvb_PipChannel)
				gmvb_MemoryBpp[B_Channel] = 24;
			else
         	gmvb_MemoryBpp[B_Channel] = 30;
      }
		#endif // (defined (ENABLE_8BIT_MEMORY))
   }
   else
   {
		#if (defined (ENABLE_8BIT_MEMORY))
      gmvb_MemoryBpp[B_Channel] = 16;
		#else // (defined (ENABLE_8BIT_MEMORY))
      if (IsHardwareLimitBpp(B_Channel))
         gmvb_MemoryBpp[B_Channel] = 16;
      else
      {
      	if(B_Channel == gmvb_PipChannel)
         	gmvb_MemoryBpp[B_Channel] = 16;
			else
				gmvb_MemoryBpp[B_Channel] = 20;
      }
		#endif // (defined (ENABLE_8BIT_MEMORY))
   }
	#else
   if (gm_GetChannelInputAttr(B_Channel, gmd_COLOR_SPACE) == gmd_RGB)
   {
      if (IsHardwareLimitBpp(B_Channel))
         gmvb_MemoryBpp[B_Channel] = 16;
      else
      {
			if(B_Channel == gmvb_PipChannel)
				gmvb_MemoryBpp[B_Channel] = 24;
      }
   }
   else
      gmvb_MemoryBpp[B_Channel] = 16;
	#endif
}

//******************************************************************************
// FUNCTION       : static void MemoryReduceCapturePrefs(gmt_PHY_CH B_Channel)
//
// USAGE          : Reduces the memory capture preferences for the current
//                  mode.  This is used when there is not enough memory
//                  available for the desired capture size.  This function
//                  will reduce the requirements (BPP, num buffers).
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MemoryBpp			(RW)
//                  gmvb_MemoryNumBuffers	(RW)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static BOOL MemoryReduceCapturePrefs(gmt_PHY_CH B_Channel)
{
   BOOL Bt_Result = FALSE;

   //
   // The logic behind this implementation:
   //
   // 1. Video is usually the same frame rate as the display so lowering the NumBuffers should not cause frame tear.
   // 2. Graphics modes have a larger variety of frame rates.  Reducing the number of buffers could cause problems.
   // 3. Video can benefit from 10-bit processing (we apply many enhancement in 10-bits) so a higher BPP is priority.
   // 4. Graphics is less likely to benefit from 10-bit processing because not much image enhancements are done to is
   //    and the source is not likely to have such a big color depth anyhow.
   if (gm_IsGraphics(B_Channel))
   {
      // Logic implemented:
      // 30bit with 3 buffers -> 24bit with 3 buffers -> 30bit with 2 buffers -> 24bit with 2 buffers ->
      //  30bit with 1 buffer -> 24bit with 1 buffer
      if (gmvb_MemoryBpp[B_Channel] == 30)
      {
         gmvb_MemoryBpp[B_Channel] = 24;
         Bt_Result = TRUE;
      }
      else if (gmvb_MemoryBpp[B_Channel] == 20)
      {
         gmvb_MemoryBpp[B_Channel] = 16;
         Bt_Result = TRUE;
      }
      else
      {
         if (gmvb_MemoryNumBuffers[B_Channel] > 1)
         {
            gmvb_MemoryNumBuffers[B_Channel]--;

            // After lowering the num of buffers, restore to higher bpp to try to get the best quality
            if  (gmvb_MemoryBpp[B_Channel] == 16)
               gmvb_MemoryBpp[B_Channel] = 20;
            else if (gmvb_MemoryBpp[B_Channel] == 24)
            {
               if (!IsHardwareLimitBpp(B_Channel))
					{
						if(B_Channel == gmvb_PipChannel)
							gmvb_MemoryBpp[B_Channel] = 24;
						else
         				gmvb_MemoryBpp[B_Channel] = 30;
      			}
            }
            Bt_Result = TRUE;
         }
         else
            Bt_Result = FALSE;
      }
   }
   else
   {
      // For video modes, it is preferred to lower the number of buffers first
      // and only lower the bpp if necessary.

      // Logic implemented:
      // 30bit with 5 buffers -> 30bit with 4 buffers -> 24bit with 5 buffers -> 24bit with 4 buffers ->
      // 30bit with 3 buffers -> 30bit with 2 buffers -> 24bit with 3 buffers -> 24bit with 2 buffers ->
      //  30bit with 1 buffer -> 24bit with 1 buffer
      if ((gmvb_MemoryNumBuffers[B_Channel] == 5) || (gmvb_MemoryNumBuffers[B_Channel] == 3))

      {
         gmvb_MemoryNumBuffers[B_Channel]--;
         Bt_Result = TRUE;
      }
      else
      {
         if  (gmvb_MemoryBpp[B_Channel] == 20)
         {
            gmvb_MemoryBpp[B_Channel] = 16;
            Bt_Result = TRUE;
         }
         else if (gmvb_MemoryBpp[B_Channel] == 30)
         {
            gmvb_MemoryBpp[B_Channel] = 24;
            Bt_Result = TRUE;
         }

         if ((Bt_Result == TRUE)
               &&
               ((gmvb_MemoryNumBuffers[B_Channel] == 4) || (gmvb_MemoryNumBuffers[B_Channel] == 2)))
         {
            // If the number of buffers was reduced but there is not enough memory such that
            // the number of bpp must be reduced, return the number of buffers to the original value.
            gmvb_MemoryNumBuffers[B_Channel]++;
         }
         else
         {
            if (gmvb_MemoryNumBuffers[B_Channel] > 1)
            {
               gmvb_MemoryNumBuffers[B_Channel]--;

               // After lowering the num of buffers, restore to higher bpp to try to get the best quality
               if  (gmvb_MemoryBpp[B_Channel] == 16)
                  gmvb_MemoryBpp[B_Channel] = 20;
               else if (gmvb_MemoryBpp[B_Channel] == 24)
					{
						if(B_Channel == gmvb_PipChannel)
							gmvb_MemoryBpp[B_Channel] = 24;
						else
			         	gmvb_MemoryBpp[B_Channel] = 30;
			      }
               Bt_Result = TRUE;
            }
         }
      }
   }

   msg("gmvb_MemoryBpp[B_Channel] = %d", gmvb_MemoryBpp[B_Channel]);
   msg("Bt_Result = %d", Bt_Result);

   return Bt_Result;
}


//******************************************************************************
//
// FUNCTION       : static void MemoryAllocationDynamic(gmt_PHY_CH B_Channel)
//
// USAGE          : Checks if there is enough memory for the channel.
//                  If there is not, uses the dynamic memory allocation
//                  strategy to be able to allocate memory for the channel.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MainChannel		(R)
//                  gmvb_PipChannel			(R)
//                  gmvb_MemoryBpp			(R)
//                  gmvb_MemoryNumBuffers	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void MemoryAllocationDynamic(gmt_PHY_CH B_Channel)
{
   if (B_Channel == gmvb_MainChannel)
   {
      BYTE B_InnerCount;
      BYTE B_OuterCount = MEMORY_ALLOCATION_SAFETY_COUNT;
      BYTE B_SavedBpp = gmvb_MemoryBpp[gmvb_PipChannel];

      //Set PIP to highest quality and reduce it if not enough memory for Main channel
      MemorySetCapturePrefs(gmvb_PipChannel);

      // For dynamic memory allocation, if there isn't enough memory for the desired
      //   number of frame buffers and bpp for Main, extra memory can be taken from Pip
      //   channel.
      // If taking away memory from PIP still is not enough, then start reducing quality on Main
      //   as the last resort.
      while (gm_MemoryIsAvailable(B_Channel) != OK) // Check if enough memory for MAIN
      {
         B_InnerCount = MEMORY_ALLOCATION_SAFETY_COUNT;

         do
         {
            if (!MemoryReduceCapturePrefs(gmvb_PipChannel)) // Reduce the memory requirement of PIP if possible
               break;

            if (--B_InnerCount == 0) // Sanity check to ensure not stuck in endless loop
               break;

         } while (gm_MemoryIsAvailable(B_Channel) != OK); // Keep reducing Pip quality until there is enough memory for Main

         // If enough memory for Main, exit loop; otherwise, reduce Main quality by one notch and start reducing Pip quality again
         if (gm_MemoryIsAvailable(B_Channel) == OK)
            break;
         else
         {
            // Reset PIP to highest quality and run the loop above to reduce Pip memory requirement
            MemorySetCapturePrefs(gmvb_PipChannel);

            if (!MemoryReduceCapturePrefs(B_Channel)) // Reduce the memory requirement of Main if possible
               break;

            if (--B_OuterCount == 0) // Sanity check to ensure not stuck in endless loop
               break;
         }
      }

      if ((B_InnerCount < MEMORY_ALLOCATION_SAFETY_COUNT) || (B_OuterCount < MEMORY_ALLOCATION_SAFETY_COUNT))
      {
         //	gmt_DECODER_ID B_PipDecoderID = gmd_DEC_CORTEZ_ID;

         if (gm_MemoryIsAvailable(B_Channel) == OK)
         {
            // If Main has enough memory and Pip blender is already on, reconfigure Pip input if
            //   there is any change in Bpp
            if (gm_IsModeComplete(gmvb_PipChannel) && (B_SavedBpp != gmvb_MemoryBpp[gmvb_PipChannel]))
            {
               ModeSetupInputChange(gmvb_PipChannel);
            }
            // Reconfigure memory since we made changes to it
            gm_MemorySetup(gmvb_PipChannel);
            gm_SyncUpdateWait(gmvb_PipChannel, INPUT_OUTPUT_CLOCK);

            //if (IsDecoderUsed(gmvb_PipChannel))
            //{
            //	gm_DecoderMemory(B_PipDecoderID, gmd_RESET);
            //}
         }
         else
         {
            // If in the end there is still not enough memory for Main, give up Pip buffer and try to fit
            //   Main only
            SetDisplayOff(gmvb_PipChannel, BACKGROUND_ONLY);

            gmvb_MemoryNumBuffers[gmvb_PipChannel] = 1;

            gm_MemorySetup(gmvb_PipChannel);
            gm_SyncUpdateWait(gmvb_PipChannel, INPUT_OUTPUT_CLOCK);

            // Reset Main to highest memory requirement and reduce from it
            MemorySetCapturePrefs(B_Channel);

            if (gm_MemoryIsAvailable(B_Channel) != OK)
               MemoryAllocationStatic(B_Channel);
         }
      }
   }
   else
   {
      // For Pip, memory is confined within the leftover memory from Main
      MemoryAllocationStatic(B_Channel);
   }
}

//******************************************************************************
//
// FUNCTION       : static void MemoryAllocationStatic(gmt_PHY_CH B_Channel)
//
// USAGE          : Checks if there is enough memory for the channel.
//                  If there is not, uses the static memory allocation
//                  strategy to be able to allocate memory for the channel.
//
// INPUT          : B_Channel - CH_A or CH_B
//
// OUTPUT         : None
//
// GLOBALS        : gmvb_MemoryBpp			(R)
//                  gmvb_MemoryNumBuffers	(R)
//
// USED_REGS      : None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
static void MemoryAllocationStatic(gmt_PHY_CH B_Channel)
{
   BYTE B_Count = MEMORY_ALLOCATION_SAFETY_COUNT;

   // Static memory allocation
   // if there isn't enough memory for the desired number of frame buffers and bpp, the
   //   requirements are lowered.
   // Under this method, the other channel is not affected and extra memory cannot be
   //   taken from it.
   do
   {
      if (!MemoryReduceCapturePrefs(B_Channel)) // reduce capture to memory size if possible
         break;
      if (--B_Count == 0) // Sanity check to ensure not stuck in endless loop
         break;
   } while (gm_MemoryIsAvailable(B_Channel) != OK); // Not enough memory

   // If even with only one buffer there is still not enough memory, then turn off blender and
   //   stops decoder (if in use) from continuing to write to the memory
   if (gm_MemoryIsAvailable(B_Channel) != OK)
   {
      gmvb_MemoryNumBuffers[B_Channel] = 1;

      if (gmvb_MemoryBufferTypes[B_Channel] & (BUF_DECODER | BUF_DECODER2))
      {
         //gmt_DECODER_ID B_DecoderID = gmd_DEC_CORTEZ_ID;
         //gm_DecoderMemory(B_DecoderID, gmd_DISABLE);

         gmvb_MemoryBufferTypes[B_Channel] &= ~(BUF_DECODER | BUF_DECODER2);
         gm_MemorySetup(B_Channel);
         gm_SyncUpdateWait(B_Channel, INPUT_OUTPUT_CLOCK);
      }
   }
}

//******************************************************************************
//
// FUNCTION		:	void MemoryAllocate(gmt_PHY_CH B_Channel)
//
// USAGE		:	Sets the memory capture preferences based on the
//					available memory.
//
// INPUT		:	B_Channel - CH_A or CH_B
//
// OUTPUT		:	None
//
// GLOBALS			: gmvb_MainChannel		(R)
// 					  gmvb_PipChannel 		(R)
// 					  gmvb_MemoryBpp			(R)
// 					  gmvb_MemoryNumBuffers (R)
//
// USED_REGS	:	None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void MemoryAllocate(gmt_PHY_CH B_Channel)
{

   // Set the maximum capture to memory size for the current mode.
   // If the PIP is limited to a certain size, this is the place to lower the memory requirements for it.
   gmvw_MemoryMaxHCapture[B_Channel] = MIN_SIZE(gmvw_InputHActiveOrg[B_Channel], PanelWidth);
   gmvw_MemoryMaxVCapture[B_Channel] = MIN_SIZE(gmvw_InputVActiveOrg[B_Channel], PanelHeight);

   MemorySetCapturePrefs(B_Channel);

#ifdef ENABLE_8BIT_MEMORY
   gm_SetRegBitsWord(MC_MEMORY_CONFIG, MAIN_8BIT_COMPRESS);
#else
   gm_ClearRegBitsWord(MC_MEMORY_CONFIG, MAIN_8BIT_COMPRESS);
#endif


   if (gm_MemoryIsAvailable(B_Channel) != OK) // Check if there is enough memory for this ideal config
   {
      // Use a different strategy based on the memory allocation method.
      if (gmc_MemoryAllocationMethod == gmd_MEMORY_ALLOCATE_DYNAMIC)
         MemoryAllocationDynamic(B_Channel);
      else
         MemoryAllocationStatic(B_Channel);
   }

}

//******************************************************************************
//
// FUNCTION		:	void MemoryInit(void)
//
// USAGE		:	Initialize the memory allocation to ensure all memory
//					clients are using some default address.
//
// INPUT		:	None
//
// OUTPUT		:	None
//
// GLOBALS		:	None
//
// USED_REGS	:	None
//
// PRE_CONDITION  :
//
// POST_CONDITION :
//
//******************************************************************************
void MemoryInit(void)
{
   gmt_BUFF_TYPE B_StaticBuffers;
   ROM gmt_MemoryAllocateParams *ptrMemoryParams;

   // Initialize the input and output window variables with some default values.
   // These values are needed to perform a default memory allocation so that
   // all memory clients are not pointing at address 0.
   stInput[CH_A].Hactive = 720;
   stInput[CH_A].Vactive = 240;
   gmvw_InputHActiveOrg[CH_A] = 720;
   gmvw_InputVActiveOrg[CH_A] = 240;
   gmvw_OutputHStart[CH_A] = 0;
   gmvw_OutputVStart[CH_A] = 0;
   gmvw_OutputHActive[CH_A] = PanelWidth;
   gmvw_OutputVActive[CH_A] = PanelHeight;

   stInput[CH_B].Hactive = 720;
   stInput[CH_B].Vactive = 240;
   gmvw_InputHActiveOrg[CH_B] = 720;
   gmvw_InputVActiveOrg[CH_B] = 240;
   gmvw_OutputHStart[CH_B] = 0;
   gmvw_OutputVStart[CH_B] = 0;
   gmvw_OutputHActive[CH_B] = PanelWidth / 4;
   gmvw_OutputVActive[CH_B] = PanelHeight / 4;

   // Initialize the memory allocation
#if (SDRAM_MEM_SIZE > 0x80000L)
   {
      // For 512MBit SDRAM, there are no memory allocation limitations
      ptrMemoryParams = &gmc_MemoryAllocateParams_Unlimited;
      //B_StaticBuffers = (BUF_OSD | BUF_AVS | BUF_VBI | BUF_DECODER);
      B_StaticBuffers = (BUF_OSD | BUF_AVS);
   }
#else
   {
      // For 256MBit SDRAM, there can be memory allocation limitations
      if (PanelType == PANEL_WUXGA)
         ptrMemoryParams = &gmc_MemoryAllocateParams_WUXGA_Panel;
      else if (PanelType == PANEL_1080P)
         ptrMemoryParams = &gmc_MemoryAllocateParams_1080P_Panel;
      else if (PanelType == PANEL_UXGA)
         ptrMemoryParams = &gmc_MemoryAllocateParams_UXGA_Panel;
      else
         ptrMemoryParams = &gmc_MemoryAllocateParams_Unlimited;

      //B_StaticBuffers = (BUF_OSD | BUF_AVS | BUF_VBI);
      B_StaticBuffers = (BUF_OSD | BUF_AVS);
   }
#endif

#if (FEATURE_OVERDRIVE == ENABLE)
   B_StaticBuffers |= BUF_LCD_OVERDRIVE;
#endif


   // Allocate memory
   if (gm_MemoryAllocate(B_StaticBuffers, ptrMemoryParams) != OK)
   {
      msg("*** SDRAM Memory Allocation Error ***", 0);
   }

   // Disable decoder memory
   //gm_DecoderMemory(CH_A, gmd_DISABLE);
   //gm_DecoderMemory(CH_B, gmd_DISABLE);

   // Initialize Main & PIP with some default values (so that memory clients are not pointing to address 0).
   gmvb_MemoryBpp[CH_A] = 16;
   gmvb_MemoryBpp[CH_B] = 16;
   //gmvb_MemoryBufferTypes[CH_A] = (BUF_CCS | BUF_MOTION_VECTORS);
   gmvb_MemoryBufferTypes[CH_A] = BUF_MOTION_VECTORS;

   gmvb_MemoryBufferTypes[CH_B] = 0;

   gmvb_MemoryNumBuffers[CH_A] = 1;
   gmvb_MemoryNumBuffers[CH_B] = 1;
   gmvw_MemoryMaxHCapture[CH_A] = MIN_SIZE(gmvw_InputHActiveOrg[CH_A], PanelWidth);
   gmvw_MemoryMaxVCapture[CH_A] = MIN_SIZE(gmvw_InputVActiveOrg[CH_A], PanelHeight);
   gmvw_MemoryMaxHCapture[CH_B] = MIN_SIZE(gmvw_InputHActiveOrg[CH_B], PanelWidth);
   gmvw_MemoryMaxVCapture[CH_B] = MIN_SIZE(gmvw_InputVActiveOrg[CH_B], PanelHeight);
   gm_MemorySetup(CH_A);
   gm_MemorySetup(CH_B);

   // Soft reset the IMP and IPP to clear the DDR write FIFO's after memory setup to ensure that if there is no sync,
   // the address will still get latched to the memory client. This fixes the jittering after an NVRAM reset on the
   // splashscreen as well as on the NO_TUNER option
   gm_SetRegBitsWord(SOFT_RESETS, (IPP_RESET | IMP_RESET | MVF_RESET | MHF_RESET | PVF_RESET | PHF_RESET));
   gm_WriteRegWord(SOFT_RESETS, 0x0);

}

//******************************************************************************
//
// FUNCTION       : void MemoryAddCustomBuffer(void)
//
// USAGE          : This is a function called by ModeSetupFullSetup to add a custom memory buffer
//						in DDR.
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
void MemoryAddCustomBuffer(gmt_CUSTOM_BUFF_TYPE B_CustomBuffer)
{
#ifdef LCDTV_PAL
   DWORD D_StartAddr = 0;
   SDWORD SD_MemFree;

   gmt_BUFF_TYPE B_BuffType;

   //check which buffers are set on CH_B
   B_BuffType = gmvb_MemoryBufferTypes[CH_B];
   B_BuffType &= ~gb_MemoryStaticBuffers;

   SD_MemFree = MemoryCalculateRemainingSDRAM();
#endif

   if ( B_CustomBuffer & BUF_PAL_AUTOINSTALL)
   {
#ifdef LCDTV_PAL
      // check if there is enough room to put PAL AutoInstall Buffer in without modifying previously set buffers
      if ( SD_MemFree > gmc_MemoryPalAutoInstallSize)
      {
         if ( gmc_MemoryAllocationMethod == gmd_MEMORY_ALLOCATE_STATIC ) // PAL autoinstall buffer placed at end of pip buffers
         {

#if 0
#if (FEATURE_DECODER == TWO_DECODER)
            if ( B_BuffType & BUF_DECODER2)
               D_StartAddr = gm_MemoryDecoder2EndAddressInBursts();
            else if ( B_BuffType & BUF_DECODER)
               D_StartAddr = gm_MemoryDecoderEndAddressInBursts();
            else if (B_BuffType & BUF_TELETEXT)
               D_StartAddr = gm_MemoryGetInfo(BUF_TELETEXT, BUF_END_ADDRESS);
            else
               D_StartAddr = gm_MemoryGetInfo(BUF_PIP, BUF_END_ADDRESS);
#else
            if ( B_BuffType & BUF_DECODER)
               D_StartAddr = gm_MemoryDecoderEndAddressInBursts();
            else if (B_BuffType & BUF_TELETEXT)
               D_StartAddr = gm_MemoryGetInfo(BUF_TELETEXT, BUF_END_ADDRESS);
            else
               D_StartAddr = gm_MemoryGetInfo(BUF_PIP, BUF_END_ADDRESS);
#endif
#endif

         }
         else  // PAL autoinstall buffer placed at end of main
         {
            D_StartAddr = gm_MemoryGetInfo(BUF_MAIN, BUF_END_ADDRESS);
         }
      }
      else // don't have enough memory available, need to make some space
      {
         gB_PAL_AutoInstall_MainChannelBuffer_TempReduction = TRUE;
         gmvb_MemoryNumBuffers[CH_A]--;

         gm_MemorySetup(CH_A);

         // here, we are assuming there are x buffers in main FRC and now we have x - 1
         // also assume that each buffer = 720*288*20 = 4,147,200 Bits which is larger than gmc_MemoryPalAutoInstallSize
         D_StartAddr = gm_MemoryGetInfo(BUF_MAIN, BUF_END_ADDRESS);

      }

      if ( D_StartAddr % 2 == 1) //odd, no good because we can not start writing to an odd address
      {
         D_StartAddr++;
      }

      gD_TunerAutoInstallSDRAMStartAddr = D_StartAddr;
      gB_MemoryCustomBufferAllocated |= BUF_PAL_AUTOINSTALL;
#endif
   }
}

//******************************************************************************
//
// FUNCTION       : void MemoryRemoveCustomBuffer(void)
//
// USAGE          : This is a function that removes the memory allocated to a custom memory buffer
//						in DDR.
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
void MemoryRemoveCustomBuffer(gmt_CUSTOM_BUFF_TYPE B_CustomBuffer)
{


   if (B_CustomBuffer & BUF_PAL_AUTOINSTALL)
   {
#ifdef LCDTV_PAL
      if (gB_PAL_AutoInstall_MainChannelBuffer_TempReduction == TRUE) //if true, we want to reset main FRC to the original before temp reduction
      {
         gmvb_MemoryNumBuffers[CH_A]++;
         gB_PAL_AutoInstall_MainChannelBuffer_TempReduction = FALSE;

         gm_MemorySetup(CH_A);
      }

      gD_TunerAutoInstallSDRAMStartAddr = 0;
      gB_MemoryCustomBufferAllocated &= ~BUF_PAL_AUTOINSTALL;
#endif
   }

}

//******************************************************************************
//
// FUNCTION       : WORD CalculateRemainingSDRAM(void)
//
// USAGE          : This is a function to calculate the remaining memory in DDR.
//
// INPUT          : None
//
// OUTPUT         : The remaining SDRAM size in bursts. For gmd_MEMORY_ALLOCATE_STATIC,
//					  this is defined as the memory after the Pip buffers, for
//					  gmd_MEMORY_ALLOCATE_DYNAMIC, this is defined as the memory between
//					  the end of Main FRC buffer and the start of the Pip FRC buffer.
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
SDWORD MemoryCalculateRemainingSDRAM(void)
{
   SDWORD SD_MemFree;
   gmt_BUFF_TYPE B_BuffType;

   //check which buffers are set on CH_B
   B_BuffType = gmvb_MemoryBufferTypes[CH_B];
   B_BuffType &= ~gb_MemoryStaticBuffers;

   if ( gmc_MemoryAllocationMethod == gmd_MEMORY_ALLOCATE_STATIC )
   {

      // free memory (from the end of pip buffers to end of memory) - (custom buffers)
      SD_MemFree = SDRAM_MEM_SIZE - gm_MemoryGetInfo(BUF_PIP, BUF_END_ADDRESS);

#if 0
      if (B_BuffType & BUF_TELETEXT)
         SD_MemFree = SDRAM_MEM_SIZE - gm_MemoryGetInfo(BUF_TELETEXT, BUF_END_ADDRESS);
#endif

   }
   else
   {
      // free memory (from end of main buffer to start of pip buffer) - (custom buffers)
      SD_MemFree = gm_MemoryGetInfo(BUF_PIP, BUF_START_ADDRESS) -
                   gm_MemoryGetInfo(BUF_MAIN, BUF_END_ADDRESS);

   }

   return SD_MemFree;
}

void gm_MemoryAdjust_VFlip(gmt_PHY_CH B_Channel, DWORD D_StartAddr, WORD W_Width, WORD W_Height)
{
   WORD W_BufferWidth;
   DWORD D_Stride;
   DWORD D_BufferEnd;
   DWORD D_EndAddr;
   WORD DMEMORY_BURST = 512;


	if(B_Channel == CH_A)
	{
	   W_BufferWidth = (WORD)((((DWORD)W_Width * gmvb_MemoryBpp[CH_A]) + (DMEMORY_BURST - 1)) / DMEMORY_BURST);

      // Calculate the maximum stride (in bursts)
      D_Stride = (DWORD)W_BufferWidth * (DWORD)W_Height;

      // Calculate the buffer end address
      D_BufferEnd = D_StartAddr + D_Stride - W_BufferWidth;

      // Write the memory buffer width
      gm_WriteRegWord(MFRC_BUF_WIDTH, W_BufferWidth);

      // Write the memory buffer stride
      gm_WriteRegWord(MFRC_BUF_STRIDE_hi, (WORD)(D_Stride >> 16));
      gm_WriteRegWord(MFRC_BUF_STRIDE_lo, (WORD)D_Stride);

      // Write the memory buffer write width
      gm_WriteRegWord(MFRC_WR_WIDTH, W_BufferWidth);

      // Write the memory buffer write start address
      gm_WriteRegWord(MFRC_WBUF_START_hi, (WORD)(D_StartAddr >> 16));
      gm_WriteRegWord(MFRC_WBUF_START_lo, (WORD)D_StartAddr);

      // Write the memory buffer write end address
      gm_WriteRegWord(MFRC_WBUF_END_hi, (WORD)(D_BufferEnd >> 16));
      gm_WriteRegWord(MFRC_WBUF_END_lo, (WORD)D_BufferEnd);

      // Write the memory buffer read width
      gm_WriteRegWord(MFRC_RD_WIDTH, W_BufferWidth);

      // Write the memory buffer read start address
      gm_WriteRegWord(MFRC_RBUF_START_hi, (WORD)(D_StartAddr >> 16));
      gm_WriteRegWord(MFRC_RBUF_START_lo, (WORD)D_StartAddr);

      // Write the memory buffer read end address
      gm_WriteRegWord(MFRC_RBUF_END_hi, (WORD)(D_BufferEnd >> 16));
      gm_WriteRegWord(MFRC_RBUF_END_lo, (WORD)D_BufferEnd);

	   gm_WriteRegWord(MFRC_PIXELS_OUT, W_Width);
      gm_WriteRegWord(MFRC_LINES_OUT, W_Height);

      // Disable memory bypass
      gm_ClearRegBitsWord(MC_MEMORY_CONFIG, MAIN_MC_BYPASS );

      //gm_WaitDisplayBlanking();

      gm_SyncUpdateWait(CH_A, OUTPUT_CLOCK);
	}
	else
	{
		#if 1
		// Calculate the maximum width (in bursts)
	    W_BufferWidth = (WORD)W_Width; //((((DWORD)W_Width * gmvb_MemoryBpp[CH_B]) + (DMEMORY_BURST - 1)) / DMEMORY_BURST);

	    // Calculate the maximum stride (in bursts)
	    D_Stride = (DWORD)W_BufferWidth * (DWORD)W_Height;

	    // Calculate the buffer end address
	    D_BufferEnd = D_StartAddr + D_Stride - W_BufferWidth;

	    // Write the memory buffer width
	    gm_WriteRegWord(PFRC_R0_BUF_WIDTH, W_BufferWidth);

	    // Write the memory buffer stride
	    gm_WriteRegWord(PFRC_BUF_STRIDE_hi, (WORD)(D_Stride >> 16));
	    gm_WriteRegWord(PFRC_BUF_STRIDE_lo, (WORD)D_Stride);

	    // Write the memory buffer write width
	    gm_WriteRegWord(PFRC_WR_WIDTH, W_BufferWidth);

	    // Write the memory buffer write start address
	    gm_WriteRegWord(PFRC_WBUF_START_hi, (WORD)(D_StartAddr >> 16));
	    gm_WriteRegWord(PFRC_WBUF_START_lo, (WORD)D_StartAddr);

	    // Write the memory buffer write end address
	    gm_WriteRegWord(PFRC_WBUF_END_hi, (WORD)(D_BufferEnd >> 16));
	    gm_WriteRegWord(PFRC_WBUF_END_lo, (WORD)D_BufferEnd);

	    // Write the memory buffer read width
	    gm_WriteRegWord(PFRC_R0_RD_WIDTH, W_BufferWidth);

	    // Write the memory buffer read start address
	    gm_WriteRegWord(PFRC_R0_RBUF_START_hi, (WORD)(D_StartAddr >> 16));
	    gm_WriteRegWord(PFRC_R0_RBUF_START_lo, (WORD)D_StartAddr);

	    // Write the memory buffer read end address
	    gm_WriteRegWord(PFRC_R0_RBUF_END_hi, (WORD)(D_BufferEnd >> 16));
	    gm_WriteRegWord(PFRC_R0_RBUF_END_lo, (WORD)D_BufferEnd);

	    // Write the memory buffer read pixels and lines
	    //gm_WriteRegWord(PFRC_R0_PIXELS_OUT, W_Width);
	    //gm_WriteRegWord(PFRC_R0_LINES_OUT, W_Height);

	    // Initailize all the memory registers related to region 1 and region 2 to 0
	    // This will prevent any artifacts from showing up when we switch from
	    // multi pip to single pip mode
	    gm_ClearRegBitsWord(PFRC_CTRL, PIP_WR_REGION);
	    gm_WriteRegWord(PFRC_R1_BUF_WIDTH, 0x00);
	    gm_WriteRegWord(PFRC_R1_RD_WIDTH, 0x00);
	    gm_WriteRegWord(PFRC_R1_RBUF_START_hi, 0x00);
	    gm_WriteRegWord(PFRC_R1_RBUF_START_lo, 0x00);
	    gm_WriteRegWord(PFRC_R1_RBUF_END_hi, 0x00);
	    gm_WriteRegWord(PFRC_R1_RBUF_END_lo, 0x00);
	    gm_WriteRegWord(PFRC_R1_PIXELS_OUT, 0x00);
	    gm_WriteRegWord(PFRC_R1_LINES_OUT, 0x00);
	    gm_WriteRegWord(PFRC_R2_BUF_WIDTH, 0x00);
	    gm_WriteRegWord(PFRC_R2_RD_WIDTH, 0x00);
	    gm_WriteRegWord(PFRC_R2_RBUF_START_hi, 0x00);
	    gm_WriteRegWord(PFRC_R2_RBUF_START_lo, 0x00);
	    gm_WriteRegWord(PFRC_R2_RBUF_END_hi, 0x00);
	    gm_WriteRegWord(PFRC_R2_RBUF_END_lo, 0x00);
	    gm_WriteRegWord(PFRC_R2_PIXELS_OUT, 0x00);
	    gm_WriteRegWord(PFRC_R2_LINES_OUT, 0x00);
		#endif

		gm_SyncUpdateWait(CH_B, OUTPUT_CLOCK);
	}
}

/*********************************  END  **************************************/
