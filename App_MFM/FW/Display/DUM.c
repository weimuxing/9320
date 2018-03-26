/*
	$Workfile:   Dum.c  $
	$Revision: 1.15 $
	$Date: 2012/08/24 06:59:23 $
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
//******************************************************************************

//==============================================================================
//
// MODULE:      Dum.c
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\all.h"


#if (DEFAULT_PANEL == LG_WQHD_LM270WQ1)
#include "..\..\Panel\WQHD\LG\LM270WQ1\LM270WQ1_DUMData.h"
#elif (DEFAULT_PANEL == LG_WHD_LM290WW1_SSA1)
#include "..\..\Panel\WHD\LG\LM290WW1\LM290WW1_DUMData.h"
#elif ((DEFAULT_PANEL == LG_WQXGA_LM300WQ5_SLA1)||(DEFAULT_PANEL == LG_WQXGA_LM300WQ6_SLA1))
#include "..\..\Panel\WQXGA\LG\LM300WQ5_SLA1\LM300WQ5_DUMData.h"
#elif (DEFAULT_PANEL == ZDT_4K2K_QFHD)
#include "..\..\Panel\4K2K\ZDT_4K2K\ZDT_4K2K_QFHD_DumData.h"
#elif (DEFAULT_PANEL == ZDT_4K2K_CMI)
#include "..\..\Panel\4K2K\ZDT_4K2K_CMI\ZDT_4K2K_CMI_DumData.h"


#elif (DEFAULT_PANEL == SAMSUNG_WXGA_LTI260AP01)
#include "..\..\Panel\Wxga\Samsung\LTI260AP01\LTI260AP01_DumData.h"

#else
//#if (DEFAULT_PANEL == LG_WUXGA_LM240WU9_SLA1)
//if not define ,use WU9 data
#include "..\..\Panel\WUxga\LG\LM240WU9_SLA1\LM240WU9_DUMData.h"	

//#pragma message "!!!!No Default DUM table include"
#endif

#if (FEATURE_DUM == ENABLE)
//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define DEBUG_MSG 0
#if DEBUG_MSG
#define msg(a,b)            gm_Print(a,b)
#else
#define msg(a,b)
#endif

#define DUM_LUT_START 0x2E000L

//*****************************************************************************
// FUNCTION     : void DisplayUniformityModule(BOOL B_EnableDUM, BYTE B_Is3Ch, BYTE B_XGRIDS, BYTE B_YGRIDS, WORD W_PanelWidth, WORD W_PanelHeight)
// USAGE        :
// INPUT        :
// OUTPUT       :
// GLOBALS      :
// USED_REGS    :
//*****************************************************************************
void DisplayUniformityModule(BOOL B_EnableDUM, BYTE B_Is3Ch, BYTE B_XGRIDS, BYTE B_YGRIDS, WORD W_PanelWidth, WORD W_PanelHeight)
{
   if (B_EnableDUM == 0)
   {
      gm_ClearRegBitsByte(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
   }
   else
   {
	   #if defined(SAVING_DATA_INTO_FLASH) || defined(SAVING_DATA_INTO_NVRAM) // Flash data saving      
      // remove warning
      B_Is3Ch = B_Is3Ch;
      B_XGRIDS = B_XGRIDS;
      B_YGRIDS = B_YGRIDS;
      W_PanelWidth = W_PanelWidth;
      W_PanelHeight = W_PanelHeight;
       #ifdef SAVING_DATA_INTO_FLASH	   
	   DUM_FlashCheck();
	   #else
	   DUM_NVRAMCheck();
	   #endif
      #else
      WORD W_XGRID_SIZE, W_YGRID_SIZE, W_RSTART_XGRID;
      DWORD DW_XGRID_IINC, DW_YGRID_IINC;

		W_XGRID_SIZE = W_PanelWidth / (B_XGRIDS -1);
		W_YGRID_SIZE = W_PanelHeight / (B_YGRIDS - 1);		

      DW_XGRID_IINC = (DWORD)(524288 / W_XGRID_SIZE);
      DW_YGRID_IINC = (DWORD)(524288 / W_YGRID_SIZE);

      W_RSTART_XGRID = (B_XGRIDS / 2);

      if (B_Is3Ch)
         gm_WriteRegWord(C9VODP_DUM_CONTROL_0, (DUM_ENABLE|DUM_3_COLOR));
      else
         gm_WriteRegWord(C9VODP_DUM_CONTROL_0, (DUM_ENABLE));

		gm_WriteRegDWord_LE(C9VODP_DUM_NUM_GRIDS, ((DWORD)B_YGRIDS << 9)|B_XGRIDS);
      gm_WriteRegDWord_LE(C9VODP_DUM_GRID_SZ, ((DWORD)W_YGRID_SIZE << 12)|(W_XGRID_SIZE));
      gm_WriteRegWord(C9VODP_DUM_GLOBAL0, 0xFFFF);
      gm_WriteRegWord(C9VODP_DUM_GLOBAL0 + 2, 0x03FF);
      gm_WriteRegWord(C9VODP_DUM_GLOBAL1, 0x1FFF);
      gm_WriteRegDWord_LE(C9VODP_DUM_XGRID_IINC, DW_XGRID_IINC);
      gm_WriteRegDWord_LE(C9VODP_DUM_YGRID_IINC, DW_YGRID_IINC);
      gm_WriteRegWord(C9VODP_DUM_RSTART, W_RSTART_XGRID);
      #endif
   }
}


void ApplyDUMTable(void)//120813 Edward for OSD apply after "tick" press
{
	//gm_Print("~~## Apply DUM %d",UserPrefDUMSwitch);
	if(IS_UNIFORMITY_GrayOut)
		DisplayUniformityModule(FALSE, FALSE, 1, 1, PanelWidth, PanelHeight);
	else
		DisplayUniformityModule(UserPrefDUMSwitch, FALSE, 1, 1, PanelWidth, PanelHeight);
	
}
gmt_RET_STAT gm_LoadDUMTable(WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data)
{
   DWORD DW_Addr;
   WORD i;
   WORD far * Wp_Dest;
   WORD W_Count, W_xCount, W_yCount;
   WORD W_DataShift;
   #ifdef SAVING_DATA_INTO_FLASH
   WORD W_Index;
   #endif   

   msg("%d", W_StartIndex);
   msg("S%d", W_Size);
   
   W_xCount = gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS) & DUM_NUM_XGRIDS;
   //gm_Printf("W_xCount = %d", W_xCount);
   //W_yCount = (WORD)((gm_ReadRegDWord(C9VODP_DUM_NUM_GRIDS) & DUM_NUM_YGRIDS) >> 9);
   //gm_Printf("W_yCount = %d", W_yCount);   

   if(gm_ReadRegWord(C9VODP_DUM_CONTROL_0) & DUM_3_COLOR)
      W_xCount *= 3;

   W_Count = W_StartIndex/2;

   if((W_Count/W_xCount)%2)
   {      
      // EVEN offset      
      W_StartIndex = 0x800 + W_Count - (((W_Count/W_xCount + 1))/2)*W_xCount;
      //gm_Printf(" Adding EVEN offset ...", 0);
      msg("E%X", W_StartIndex);
   }
   else
   {
      W_StartIndex = W_Count - ((W_Count/W_xCount)/2)*W_xCount;
      msg("O%X", W_StartIndex);
   }

   W_DataShift = W_Count % W_xCount;
   
   #ifdef SAVING_DATA_INTO_FLASH
   W_Index = W_StartIndex * 2;
   #endif    
   
   DW_Addr = DUM_LUT_START + W_StartIndex * 2;      

   Wp_Dest = LINEAR_TO_FPTR_W(DW_Addr);

   for (i = 0; i < W_Size; i++)
   {
	   if((((W_Count+i)/W_xCount)%2) && (DW_Addr < 0x2F000))
      {
         W_StartIndex = (W_Count + i) - ((((W_Count + i)/W_xCount + 1))/2)*W_xCount;
         DW_Addr = DUM_LUT_START + 0x1000 + W_StartIndex * 2; // EVEN offset
         Wp_Dest = LINEAR_TO_FPTR_W(DW_Addr);
         //gm_Printf(" change to EVEN offset  ...", 0);
         #ifdef SAVING_DATA_INTO_FLASH
         W_Index = W_StartIndex*2 + 0x1000;
         #endif 
         W_DataShift = 0;
      }
      else if(((((W_Count+i)/W_xCount)%2) == 0) && (DW_Addr >= 0x2F000))
      {
         W_StartIndex = (W_Count + i) - (((W_Count + i)/W_xCount)/2)*W_xCount;
         DW_Addr = DUM_LUT_START + W_StartIndex * 2; // ODD offset
         Wp_Dest = LINEAR_TO_FPTR_W(DW_Addr);
         //gm_Printf(" change to ODD offset  ...", 0);
         #ifdef SAVING_DATA_INTO_FLASH
         W_Index = W_StartIndex*2;
         #endif         
         W_DataShift= 0;
      }
      else
      {
         //gm_Printf("W_StartIndex = 0x%X", W_StartIndex);
      }
      
      Wp_Dest[((W_Count + i)%W_xCount)-W_DataShift] = Wp_Data[i];
      
      #ifdef SAVING_DATA_INTO_FLASH
      //asm{ cli }            
      //FlashWriteWord(W_Index+i*2+0xBFD000, Wp_Data[i]);            
      FlashWriteWord(W_Index+(((W_Count + i)%W_xCount)-W_DataShift)*2+0x5FD000, Wp_Data[i]);
      //asm{ sti }
      #endif      
      
	  #if 0//def SAVING_DATA_INTO_NVRAM
      //asm{ cli }            
      //FlashWriteWord(W_Index+i*2+0xBFD000, Wp_Data[i]);            
      NVRam_WriteWORD(W_Index+(((W_Count + i)%W_xCount)-W_DataShift)*2+UniformityIndexODDStart, Wp_Data[i]);
      //asm{ sti }
	  #endif		
      
   }

   return OK;
}

void DUM_ClearLut(void)
{
   // Clear DUM ram
   _fmemset(LINEAR_TO_FPTR(DUM_LUT_START),0xFF,0x2000);
}

#ifdef SAVING_DATA_INTO_FLASH
void DUM_FlashCheck(void)
{
   DWORD DW_FlashAddr = 0x5FC000; // 0xBFC000; // Flash mapping is changed in CUT2
   WORD W_RegAddr;
   WORD W_TableIndex;
   DWORD W_Total;
   DWORD W_DataChange;
   WORD far * Wp_Dest;
   
   if((FlashReadByte(DW_FlashAddr)&BIT0) && (FlashReadByte(DW_FlashAddr) != 0xFF))
   {      
      msg(" ~~~~ Program DUM data from flash 0x%x...", FlashReadByte(DW_FlashAddr));

      // Registers Programming
      for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
      {
         gm_WriteRegWord(W_RegAddr, FlashReadWord(DW_FlashAddr));
         if((W_RegAddr == C9VODP_DUM_CONTROL_0) && 
            (gm_ReadRegWord(C9VODP_DUM_CONTROL_0)&DUM_ENABLE) && 
            (UserPrefDUMSwitch == FALSE))
         {
            UserPrefDUMSwitch = TRUE;
         }
         W_RegAddr += 2;
         DW_FlashAddr += 2;
      }      

      gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Disable DUM_ENABLE for ram write
      
      W_Total = (gm_ReadRegDWord_LE(C9VODP_DUM_NUM_GRIDS) >> 9) * (gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS)&DUM_NUM_XGRIDS);      
      W_DataChange = (((gm_ReadRegDWord_LE(C9VODP_DUM_NUM_GRIDS) >> 9)+1)/2) * (gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS)&DUM_NUM_XGRIDS);
      
      if(gm_ReadRegWord(C9VODP_DUM_CONTROL_0) & DUM_3_COLOR)
      {
         W_Total *= 3;      
         W_DataChange *= 3;
      }      
      
      msg("W_Total = %d", W_Total);
      msg("W_DataChange = %d", W_DataChange);

      DW_FlashAddr = 0x5FD000; // Odd data

      Wp_Dest = LINEAR_TO_FPTR_W(0x2E000);
      
      // Data Programming
      for(W_TableIndex = 0; W_TableIndex < W_Total;)
      {      
         Wp_Dest[W_TableIndex] = FlashReadWord(DW_FlashAddr + W_TableIndex*2); // 0xBCF000);
         W_TableIndex++;

         // even data
         if((W_TableIndex == W_DataChange) && (DW_FlashAddr == 0x5FD000))
         {
            W_TableIndex = 0;
            Wp_Dest = LINEAR_TO_FPTR_W(0x2F000);
            DW_FlashAddr += 0x1000;
         }
      }

      gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Enable DUM_ENABLE after ram write done ...
     
   }
   else
   {
      msg(" ~~~~ DUM is not enable or there is no data saved in flash ...", 0);
      DisplayUniformityModule(FALSE, FALSE, 1, 1, PanelWidth, PanelHeight);
   }
}
#endif // #ifdef SAVING_DATA_INTO_FLASH


#ifdef SAVING_DATA_INTO_NVRAM
void DUM_NVRAMCheck(void)
{
   WORD DW_NvramAddr ;//= UniformityRegisterSettingStart; // 0x
   WORD W_RegAddr;
   WORD W_TableIndex,X_GRID=0,ODD_Total=0,EVEN_Total=0;
   DWORD W_Total;
   DWORD W_DataChange;
   WORD far * Wp_Dest;
   BYTE Dum_Buf[4096], W_Addr,Y_GRID=0;

   if(UserPrefDUMSwitch == UNIFORMITY_Table1)
	 DW_NvramAddr= UniformityRegisterSettingStart;
   else  //table2
	 DW_NvramAddr= UniformityRegisterSettingStart2;

   if((NVRam_ReadByte(DW_NvramAddr)&BIT0) && (NVRam_ReadByte(DW_NvramAddr) != 0xFF))
   {      
      msg(" ~~~~ Program DUM data from Nvram 0x%x...", NVRam_ReadByte(DW_NvramAddr));

      // Registers Programming
      for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
      {
         gm_WriteRegWord(W_RegAddr, NVRam_ReadWORD(DW_NvramAddr));
		 #if 0
         if((W_RegAddr == C9VODP_DUM_CONTROL_0) && 
            (gm_ReadRegWord(C9VODP_DUM_CONTROL_0)&DUM_ENABLE) && 
            (UserPrefDUMSwitch == FALSE))
         {
            UserPrefDUMSwitch = TRUE;
         }
		 #endif
         W_RegAddr += 2;
         DW_NvramAddr += 2;
      }      

      gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Disable DUM_ENABLE for ram write
      
	  X_GRID=(gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS)&DUM_NUM_XGRIDS);
	  Y_GRID=(BYTE)(gm_ReadRegDWord_LE(C9VODP_DUM_NUM_GRIDS) >> 9);
      W_Total = X_GRID*Y_GRID;//(gm_ReadRegDWord_LE(C9VODP_DUM_NUM_GRIDS) >> 9) * (gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS)&DUM_NUM_XGRIDS);      
      W_DataChange = (((gm_ReadRegDWord_LE(C9VODP_DUM_NUM_GRIDS) >> 9)+1)/2) * (gm_ReadRegWord(C9VODP_DUM_NUM_GRIDS)&DUM_NUM_XGRIDS);
      
	  if((Y_GRID%2) !=0)
	  	ODD_Total=1;

	  if(ODD_Total==1)
	 	 ODD_Total = X_GRID * (Y_GRID/2+1);
	  else
	  	ODD_Total = X_GRID * (Y_GRID/2);
	  EVEN_Total = X_GRID * (Y_GRID/2);
	  
      if(gm_ReadRegWord(C9VODP_DUM_CONTROL_0) & DUM_3_COLOR)
      {
         W_Total *= 3;      
		 EVEN_Total *=3;
		 ODD_Total *=3;
         W_DataChange *= 3;
      }      
      
      msg("W_Total = %d", W_Total);
      msg("W_DataChange = %d", W_DataChange);

	  if(UserPrefDUMSwitch == UNIFORMITY_Table1)
	  	DW_NvramAddr= UniformityIndexODDStart;
	  else	//table2
		DW_NvramAddr= UniformityIndexODDStart2;

 //     DW_NvramAddr = UniformityIndexODDStart; // Odd data

      Wp_Dest = LINEAR_TO_FPTR_W(0x2E000);
      
	  NVRam_ReadBuffer(DW_NvramAddr, Dum_Buf, 4096);
	  
      // Data Programming
      for(W_TableIndex = 0; W_TableIndex < ODD_Total;)
      {      
         Wp_Dest[W_TableIndex] = Dum_Buf[(W_TableIndex)*2]|Dum_Buf[(W_TableIndex)*2+1]<<8;
         W_TableIndex++;
	  }	
	  
	  if(UserPrefDUMSwitch == UNIFORMITY_Table1)
	  	DW_NvramAddr= UniformityIndexEvenStart;
	  else	//table2
		DW_NvramAddr= UniformityIndexEvenStart2;
      //DW_NvramAddr = UniformityIndexEvenStart; // Odd data

            Wp_Dest = LINEAR_TO_FPTR_W(0x2F000);
        
	  NVRam_ReadBuffer(DW_NvramAddr, Dum_Buf, 4096);
	  
      // Data Programming
      for(W_TableIndex = 0; W_TableIndex < EVEN_Total;)
      {      
         Wp_Dest[W_TableIndex] = Dum_Buf[(W_TableIndex)*2]|Dum_Buf[(W_TableIndex)*2+1]<<8;
         W_TableIndex++;
      }

      gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Enable DUM_ENABLE after ram write done ...
     
   }
   else
   {
      msg(" ~~~~ DUM is not enable or there is no data saved in flash ...", 0);
      DisplayUniformityModule(FALSE, FALSE, 1, 1, PanelWidth, PanelHeight);
	  #if 0//def Qisda_93XX_BOARD	//120809 Edward enable for all model
	  if(UserPrefDUMSwitch == UNIFORMITY_Table1) //Table1
	  {
	  	  W_Addr=0;
		  // Registers Programming
		  for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
		  {
			 gm_WriteRegWord(W_RegAddr, (DUM_1_Register[W_Addr]|DUM_1_Register[W_Addr+1]<<8));
			 W_RegAddr += 2;
			 W_Addr += 2;
		  } 	 
		  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Disable DUM_ENABLE for ram write

		  Wp_Dest = LINEAR_TO_FPTR_W(0x2E000);
		  for(W_TableIndex = 0; W_TableIndex < 4096;)
		  { 	 
			 Wp_Dest[W_TableIndex] = DUM_1_ODD[(W_TableIndex)*2]|DUM_1_ODD[(W_TableIndex)*2+1]<<8;
			 W_TableIndex++;
		  }		  	
		  Wp_Dest = LINEAR_TO_FPTR_W(0x2F000);
		  for(W_TableIndex = 0; W_TableIndex < 4096;)
		  { 	 
			 Wp_Dest[W_TableIndex] = DUM_1_EVEN[(W_TableIndex)*2]|DUM_1_EVEN[(W_TableIndex)*2+1]<<8;
			 W_TableIndex++;
		  }		  	
		  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Enable DUM_ENABLE after ram write done ...
	  }
	  if(UserPrefDUMSwitch == UNIFORMITY_Table2) //Table2
     {
	  	  W_Addr=0;
		  // Registers Programming
		  for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
		  {
			 gm_WriteRegWord(W_RegAddr, (DUM_2_Register[W_Addr]|DUM_2_Register[W_Addr+1]<<8));
			 W_RegAddr += 2;
			 W_Addr += 2;
		  } 	 
		  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Disable DUM_ENABLE for ram write

		  Wp_Dest = LINEAR_TO_FPTR_W(0x2E000);
		  for(W_TableIndex = 0; W_TableIndex < 4096;)
		  { 	 
			 Wp_Dest[W_TableIndex] = DUM_2_ODD[(W_TableIndex)*2]|DUM_2_ODD[(W_TableIndex)*2+1]<<8;
			 W_TableIndex++;
		  }		  	
		  Wp_Dest = LINEAR_TO_FPTR_W(0x2F000);
		  for(W_TableIndex = 0; W_TableIndex < 4096;)
		  { 	 
			 Wp_Dest[W_TableIndex] = DUM_2_EVEN[(W_TableIndex)*2]|DUM_2_EVEN[(W_TableIndex)*2+1]<<8;
			 W_TableIndex++;
		  }		  	
		  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Enable DUM_ENABLE after ram write done ...
     }
	  #endif
   }
}

void DUM_SAVEtoNVRAM(void)
{
	BYTE far * Wp_Dest;
	WORD DW_NvramAddr = UniformityIndexODDStart; // 0x
#ifdef Gamma_DUM_Initial_Check
	BYTE B_temp[28];
	DWORD DW_LUTAddr;
	WORD W_RegAddr,i=0,DUM_CheckSum=0;
#endif

	Wp_Dest = LINEAR_TO_FPTR_B(0x2E000);	//ODD 0x2E
	NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 4096);
	
#ifdef Gamma_DUM_Initial_Check
	DUM_CheckSum = CalculateChecksum_far(Wp_Dest,4096);
#endif
	
	Wp_Dest = LINEAR_TO_FPTR_B(0x2F000);	//EVEN 0x2F
	DW_NvramAddr = UniformityIndexEvenStart;
	NVRam_WriteBuffer_DUM_Gamma(DW_NvramAddr, Wp_Dest, 4096);

#ifdef Gamma_DUM_Initial_Check
	DUM_CheckSum += CalculateChecksum_far(Wp_Dest,4096);

	for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
	{
		B_temp[i*2]=gm_ReadRegWord(W_RegAddr)&0xFF ;
		B_temp[i*2+1]=gm_ReadRegWord(W_RegAddr)>>8 ;	
		i++;
		W_RegAddr += 2;
	}
	DUM_CheckSum += CalculateChecksum(B_temp,28);
	DUM_CheckSum +=1;	//for enable bit0

	DW_NvramAddr = UniformityIndexODDStart-1;
	NVRam_WriteByte(DW_NvramAddr,DUM_CheckSum );
#endif

}


void InitializeGammaNvram(BYTE C_Selected_Profile) // Clear Gamma NVRAM
{	
	int i;
	BYTE buf[1000]= {0xff};
	WORD DW_NvramAddr;
	DW_NvramAddr = C_GAMMA_COMPONENT_SAVED_START_OFFSET + (0x2000*C_Selected_Profile);
	
	for(i=0;i<1000;i++)
		buf[i]=0xff;
	
	for(i=0; i<8193;)
	{
		NVRam_WriteBuffer(DW_NvramAddr+i, buf, 1000);
		i+=1000;
	}

	gm_Print("ERASE Gamma data Done",0);
}
#endif // #ifdef SAVING_DATA_INTO_NVRAM

#endif
