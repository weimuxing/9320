/*
	$Workfile:   FLashSpiPlus.C  $
	$Revision: 1.10 $
	$Date: 2013/01/29 10:34:23 $
*/
#define __FLASHSPI_C__
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
// MODULE:      flashspiPLUS.c
//
// USAGE:       This module contains the specific SPI flash memory driver code
//
//
//******************************************************************************

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include <math.h>
#include <string.h>
#include "System\All.h"


#ifdef SPIFLASH_CTRL_DRV
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
//void gm_Printf(const far char *Bp_String, ...);

//#define SPIFLASH_DEBUG
//#define SPIFLASH_DEBUG_EXT

#ifdef SPIFLASH_DEBUG
#define  msg(a)             		gm_Print(a,0)
#define  msg_i(a,b)         		gm_Print(a,b)
#define  msg_ii(a,b,c)         		gm_Printf(a,b,c)
#define  msg_iii(a,b,c,d)        	gm_Printf(a,b,c,d)
#else
#define  msg(a)
#define  msg_i(a,b)
#define  msg_ii(a,b,c)
#define  msg_iii(a,b,c,d)
#endif

#ifdef SPIFLASH_DEBUG_EXT
#define  msgx(a)             		gm_Print(a,0)
#define  msgx_i(a,b)         		gm_Printf(a,b)
#define  msgx_ii(a,b,c)         	gm_Printf(a,b,c)
#define  msgx_iii(a,b,c,d)        gm_Printf(a,b,c,d)
#else
#define  msgx(a)
#define  msgx_i(a,b)
#define  msgx_ii(a,b,c)
#define  msgx_iii(a,b,c,d)
#endif

//#define ShortDelay() asm {nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;}

#if (FLASH_SIZE > (1024*1024*4))
#pragma message "Unsupported big flash size !"
#endif

//Address shift (on the driver level only for serial flash)
#ifndef __ADDR24__ //20bit mode shift
#if (FLASH_SIZE >= (1024*1024)) //>=1M
#define ADDR_SHIFT 0x10000L
#else
#define ADDR_SHIFT (FLASH_SIZE -0xF0000UL)
#endif
#else	//24bit mode shift
#define ADDR_SHIFT 0
#endif

//
// Send data byte to Flash to specific address. This is part of
// erase and write algorithm
//
#define InitCRC	0x1021
#define PolDiv	0x1021

#define SpiCheckAccess() ((gm_ReadRegWord(SERIAL_FLASH_CTRL) & 0x08) !=0) //TRUE if busy

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
//static void SpiCmdStart(void);
#define SpiCmdStart() 	gm_WriteRegWord(CACHE_SPI_CTRL, 0x01)

//static void SpiCmdStop(void);
#define SpiCmdStop()	gm_WriteRegWord(CACHE_SPI_CTRL, 0x00)

static BOOL SpiWrite(BYTE B_Data);
static BOOL SpiPageProgram(DWORD Dw_Addr, BYTE * pB_Data, WORD W_Length);
static BOOL SpiRead(BYTE * B_RD_Data);
static BOOL SpiWriteProtectionDisable(void);
static BOOL SpiWriteProtectionEnable(void);
static BOOL SpiWriteEnable(void);
static BOOL SpiReadStatusRegister(BYTE * B_StatusReg);
static BOOL SpiStatusPoll(WORD W_TimeOut);
static BOOL SpiReadBurst(DWORD Dw_StartAddr, BYTE B_Length, BYTE * Ba_Buf);
//static BOOL SpiProgram(DWORD Dw_Addr, BYTE B_Data);
static BOOL SpiChipErase(void);
static BOOL SpiCheckId(BYTE * Bp_Id);
static pChipFeatures_t SpiGetChipFeatures(void);
static BOOL SpiSetChipFeatures(void);
static BOOL SpiSectorErase(DWORD Dw_Addr);
BOOL EnableWriteProtection(void);
BOOL ClearWriteProtection(void);



//static BOOL SpiWriteProtectionDisable_Winbond(void);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

FLASHDRIVER DefaultDriver =
{
   SpiWrite,
   SpiPageProgram,
   SpiRead,
   SpiWriteProtectionDisable,
   SpiWriteProtectionEnable,
   SpiWriteEnable,
   SpiReadStatusRegister,
   SpiStatusPoll,
   SpiReadBurst,
   SpiChipErase,
   SpiSectorErase
};

FLASHDRIVER WinbondDriver =
{
   SpiWrite,
   SpiPageProgram,
   SpiRead,
   SpiWriteProtectionDisable,
   SpiWriteProtectionEnable,
   SpiWriteEnable,
   SpiReadStatusRegister,
   SpiStatusPoll,
   SpiReadBurst,
   SpiChipErase,
   SpiSectorErase
};

FLASHCOMMANDS DefaultCommands =
{
   0x06,		/* write enable command */
   0x02,		/* byte program command */
   0x04,		/* WRITE disable command*/
   0x05,		/* read status command*/
   0x50,		/* enable write status register command*/
   0x01,		/* write status register command*/
   0x0B,		/* High speed read command*/
   0x60,		/* chip erase command */
   0x20,    /* Sector erase command */
};


FLASHCOMMANDS WinbondCommands =
{
   0x06,		/* write enable command */
   0x02,		/* byte program command */
   0x04,		/* WRITE disable command*/
   0x05,		/* read status command*/
   0x50,		/* enable write status register command*/
   0x01,		/* write status register command*/
   0x0B,		/* High speed read command*/
   0xC7,		/* erase command */
   0x20,    /* Sector erase command */
};


ChipFeatures_t const ChipFeaturesTable[] =
{
   //SPI chip MXIC MX25L1605A

   {	0xc2,			// manufacturer id
      0x14,			// device id
      0x200000UL,	// size
      256,			// page size
      0x1000,     // Sector Size
      &DefaultDriver,	// Driver functions
      &DefaultCommands	//Driver commands
   },

   //MXIC MX25L3205
   { 0xc2,			// manufacturer id
     0x15,			// device id
     0x400000UL,		// size
     256,			// page size
     0x1000,      // Sector Size
     &DefaultDriver,	// Driver functions
     &DefaultCommands	//Driver commands
   },

   // SST25VF016B
   {
      0xBF,			// manufacturer id
      0x41,			// device id
      0x200000UL,	// size
      1,				// page size
      0x1000,      // Sector Size
      &DefaultDriver,	// Driver functions
      &DefaultCommands	//Driver commands
   },

   // SST25VF032B
   {
      0xBF,       // manufacturer id
      0x4A,       // device id
      0x400000UL, // size
      1,          // page size
      0x1000,      // Sector Size
      &DefaultDriver,   // Driver functions
      &DefaultCommands  //Driver commands
   },


   //Winbond W25X16VSIG
   {
      0xEF,			// manufacturer id
      0x14,			// device id
      0x200000UL,	// size
      256,			// page size
      0x1000,     // Sector Size
      &WinbondDriver,	// Driver functions
      &WinbondCommands	//Driver commands
   },


   //Winbond W25X32VSIG /W25Q32BVSIG
   {
      0xEF,       // manufacturer id
      0x15,       // device id
      0x400000UL, // size
      256,        // page size
      0x1000,     // Sector Size
      &WinbondDriver,   // Driver functions
      &WinbondCommands  //Driver commands
   },

   //Atmel AT26DF321
   {
      0x1f,          // manufacturer id
      0x47,          // device id
      0x400000UL,    // size
      256,           // page size
      0x1000,        // Sector Size
      &DefaultDriver,   // Driver functions
      &DefaultCommands  //Driver commands
   },

   //Default SPI chip features
   {
      0, 				// manufacturer id
      0, 				// device id
      FLASH_SIZE, 	// size
      PAGE_SIZE,		// page size
      0x1000,         // Sector Size
      &DefaultDriver,	// Driver functions
      &DefaultCommands	//Driver commands
   } //end of table, default values;
};


//******************************************************************************
//  S T A T I C    V A R I A B L E S
//******************************************************************************
//static BYTE bTimer_1ms;


//******************************************************************************
//  C O D E
//******************************************************************************

void ShortDelay(void)
{
   /* add some delay here */
   WORD WDel = 100;
   while (--WDel);
}

//******************************************************************************
//
// FUNCTION     :   BOOL Flash_EraseBlockX(void)
// USAGE        :   Erase flash sector. Erases the whole device in this version.
//					     Blank check the whole flash in the end.
// OUTPUT		 :	  TRUE if successful, FALSE if ERROR
// INPUT        :   None
// GLOBALS      :   None
// USED_REGS    :   None
//
//******************************************************************************
BOOL Flash_EraseBlockX(void)
{
   BYTE Bp_Id[2];
   pChipFeatures_t pChip = SpiGetChipFeatures();

   SpiCmdStop();  //initialize SPI Interface

   if (!SpiCheckId(Bp_Id)) // return an error when unsuccessful chip ID verification
      return FALSE;

   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT))
   {
      if (pChip->Driver->WriteProtectionDisable())
      {
         /* add some delay here */
         {
            DWORD WDel = 0x1ffffL;
            while (--WDel);
         }

         if (pChip->Driver->ChipErase())
         {
            if (pChip->Driver->StatusPoll(ERASE_TIMEOUT))
            {
               return TRUE;
            }
         }
      }
   }
   return FALSE;
}


//******************************************************************************/
//
// FUNCTION : BOOL Flash_EraseSector(DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode)
// USAGE    : Erase SPI secotr memory 
// INPUT    : DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
BOOL Flash_EraseSector(DWORD Dw_Addr,WORD SectorCnt, BYTE ErMode)
{
   pChipFeatures_t pChip = SpiGetChipFeatures();
   BYTE Bp_Id[2];

   gm_Delay10ms(1);  //Fixed GetChipID fialed in MXIC  4MB flash
   
   if(ErMode ==SECT_ER_ALL)
      return Flash_EraseBlockX();
      
      
   SpiCmdStop();  //initialize SPI Interface

   if (!SpiCheckId(Bp_Id)) // return an error when unsuccessful chip ID verification
      return FALSE;

   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT)!=TRUE)
   {
      return FALSE;
   }
      
   if (pChip->Driver->WriteProtectionDisable()!=TRUE)
   {
      msgx("SE1");
      return FALSE;
   }

/*
   if (pChip->Driver->WriteEnable()!=TRUE)
   {
      msgx("SE2");
      return FALSE;//error      
   }
*/

   if(ErMode == SECT_ER_DEST_INCLUDED)
   {
      while(SectorCnt--)
      {
         if(pChip->Driver->SectorErase(Dw_Addr)!=TRUE)
         {
            msgx_i("SE3(0x%lX)",Dw_Addr);
            return FALSE;
         }
/*
         if (pChip->Driver->StatusPoll(SECT_ERASE_TIMEOUT) != TRUE)
         {
            msgx("SE4");
            return FALSE; //  execute poling
         }
*/         
         Dw_Addr+=pChip->W_SectorSize;         
      }
   }
   else if(ErMode == SECT_ER_DEST_EXCLUDED)
   {
      WORD TotalSectr =(WORD) (pChip->DW_Size / pChip->W_SectorSize);  
      DWORD Dw_AddrX=0;
      while(TotalSectr--)
      {
         if((SectorCnt) && (Dw_Addr>=Dw_AddrX) && (Dw_Addr < Dw_AddrX+pChip->W_SectorSize))
         {  
            SectorCnt--;
            Dw_Addr+=pChip->W_SectorSize;
         }
         else
         {
            if(pChip->Driver->SectorErase(Dw_AddrX)!=TRUE)
            {
               msgx_i("SE5(0x%lX)",Dw_AddrX);
               return FALSE;
            }
/*            
            if (pChip->Driver->StatusPoll(SECT_ERASE_TIMEOUT) != TRUE)
            {
               msgx("SE6");
               return FALSE; //  execute poling
            }
*/            
         }
         Dw_AddrX+=pChip->W_SectorSize;
      }
   }
   else
   {
      msgx("SE7");
      return FALSE;
   }
   return TRUE;
}


//******************************************************************************
//
// FUNCTION :BOOL WriteBuf2Flash(DWORD StartAddr,WORD length, BYTE * p_Buf)
// USAGE    :Write data to SPI Flash
// INPUT    :(DWORD StartAddr,WORD length, BYTE * p_Buf)
// OUTPUT   :TRUE if successful, FALSE if ERROR
// GLOBALS  :None
// USED_REGS:None
//
//******************************************************************************
BOOL WriteBuf2Flash(DWORD StartAddr,WORD length, BYTE * p_Buf)
{
   WORD W_Len;
   pChipFeatures_t pChip = SpiGetChipFeatures();
   WORD W_PageSize = pChip->W_Page ;

   union
   {
      DWORD dw;
      BYTE b[4];
      WORD w[2];
   } FlashAddr;

   //initialize SPI Interface
   SpiCmdStop();

   FlashAddr.dw = StartAddr;
   FlashAddr.dw += ADDR_SHIFT;
   FlashAddr.dw %= pChip->DW_Size;

   for (;length; FlashAddr.dw+=(DWORD)W_Len, p_Buf+=W_Len) 								// Write byte
   {

      if ((FlashAddr.b[0] & (W_PageSize-1))==0) // if flash address is aligned to flash address page border
      {
         W_Len = (length >= W_PageSize) ? W_PageSize : length;
      }
      else
      {
         // if flash address is not aligned to border then calculate the size
         // next iteration will have address aligned
         WORD W_MaxSize = (W_PageSize-FlashAddr.b[0] & (W_PageSize-1));
         W_Len = (length<(WORD)W_MaxSize) ? length : W_MaxSize;
      }
      length -= W_Len;

      if (pChip->Driver->PageProgram(FlashAddr.dw, p_Buf,W_Len)==FALSE)
         return FALSE;
   }
   return TRUE;
}

//******************************************************************************
//
// FUNCTION : BOOL SerialFlashInit(WORD W_DeviceID)
// USAGE    : initiate ISP registers and check...
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
BOOL SerialFlashInit(void)
{

   SpiCmdStop();
   gm_TimerStart(WORK1_1MS_TMR,1);
   while ((gm_ReadRegByte(SERIAL_FLASH_STATUS)&0x01)!=0)
   {
      if (gm_TimerCheck(WORK1_1MS_TMR)!=TIMER_OK) return FALSE;//timeout
   }

   // Check SPI memory ID
   return SpiSetChipFeatures();

}

BYTE FlashWriteByte(DWORD DestAddr, BYTE Data)
{
	BOOL Bl_Error = TRUE;// successful;

	//initialize SPI Interface
	SpiCmdStop(); 

	//check if memory is not busy
	if (SpiStatusPoll(WRITE_TIMEOUT) != TRUE) 
		return FALSE; //  execute poling 

	if(SpiWriteProtectionDisable() != TRUE)
		return FALSE; // FLASH_WRITE_FAILED;

	//check if memory is not busy
	if (SpiStatusPoll(WRITE_TIMEOUT) != TRUE) 
		return FALSE; //  execute poling 
	
	if (SpiWriteEnable() != TRUE) 
		return FALSE; 

	SpiCmdStart();

	// 0x02 BYTE Program
	if(SpiWrite(0x02) != TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) (DestAddr >> 16)) != TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) (DestAddr >> 8)) != TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) DestAddr) != TRUE) Bl_Error = FALSE;
	else
		{
			if(SpiWrite(Data) != TRUE)
			{
				Bl_Error = FALSE;
			}
		}
		
	SpiCmdStop();
	
	return Bl_Error;	
}


BYTE FlashWriteWord(DWORD DestAddr, WORD Data)
{  
	BOOL Bl_Error = TRUE;// successful;

	Bl_Error = FlashWriteByte(DestAddr, ((BYTE)(Data)));
	
	Bl_Error = FlashWriteByte((DestAddr+1), ((BYTE)(Data >> 8)));

	if (SpiStatusPoll(WRITE_TIMEOUT) != TRUE)
		return FALSE; //  execute poling 
	
	return Bl_Error;	
}

//******************************************************************************
//
// FUNCTION :BOOL SpiWrite(BYTE B_Data)
// USAGE    :Generates a byte transmission sequence at the SPI interface
// INPUT    :BYTE value to be received
// OUTPUT   :TRUE if successful, FALSE if ERROR
// GLOBALS  :None
// USED_REGS:None
//
//******************************************************************************
static BOOL SpiWrite(BYTE B_Data)
{
   gm_TimerStart(WORK1_1MS_TMR,2);
   while (SpiCheckAccess()==TRUE)
   {
      if (gm_TimerCheck(WORK1_1MS_TMR)== TIMER_OK) continue;
      return FALSE;//Timeout error
   }
   gm_WriteRegByte (SERIAL_FLASH_WR_DATA, B_Data);
   gm_WriteRegWord (SERIAL_FLASH_CTRL, 0x08); // SPI_ACCESS
   return TRUE; //success
}




//******************************************************************************
//
// FUNCTION : void SpiWrite(BYTE B_Data)
// USAGE    : Reseives a byte value from SPI interface
// INPUT    : BYTE value to be received to SPI
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiRead(BYTE * pB_RD_DATA)
{
   // Wait until hardware is ready -- Maybe better is satring timer outside of this function?
   gm_TimerStart(WORK1_1MS_TMR,3);
   while (SpiCheckAccess()==TRUE)
   {
      if (gm_TimerCheck(WORK1_1MS_TMR)== TIMER_OK) continue;
      return FALSE;//Timeout error
   }

   gm_WriteRegWord(SERIAL_FLASH_CTRL, 0x08); // SPI_ACCESS

   // Wait until SPI bus circle is completed;
   while (SpiCheckAccess()==TRUE)
   {
      if (gm_TimerCheck(WORK1_1MS_TMR)== TIMER_OK) continue;
      return FALSE;//Timeout error
   }

   *pB_RD_DATA = (BYTE)gm_ReadRegWord(SERIAL_FLASH_RD_DATA);
   return TRUE;
}

//******************************************************************************
//
// FUNCTION : BOOL SpiReadStatusRegister(BYTE * B_StatusReg)
// USAGE    :
// INPUT    : Status registister store space pointer
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiReadStatusRegister(BYTE * B_StatusReg)
{
   BOOL Bl_Error = TRUE;//successful
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   SpiCmdStart();
//	ShortDelay();

   if (SpiWrite (pChip->Commands->RDSR)!=TRUE)
   {
      msgx_i("SRS1",0);
      Bl_Error = FALSE; // error
   }
   else if (SpiRead(B_StatusReg)!=TRUE)
   {
      msgx_i("SRS2",0);
      Bl_Error = FALSE; //error
   }

// ShortDelay();
   SpiCmdStop();
   return Bl_Error;
}

//******************************************************************************
//
// FUNCTION : BYTE SpiStatusPoll(W_TimeOut)
// USAGE    :
// INPUT    : Timeout value in 10ms ; if W_TimeOut == 0 timer is not restarted
// OUTPUT   : TRUE if successful (Write OK), FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiStatusPoll (WORD W_TimeOut)
{
   BYTE B_Status;
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   if (pChip->Driver->ReadStatusRegister(&B_Status)!=TRUE)
   {
      msgx_i("SSP1",0);
      return FALSE; // error SPI access
   }
   if ((B_Status & 0x01)== 0x00)
      return TRUE; // successful; polling OK

   //restart timer for setiing timeout value
   if (W_TimeOut != 0)
      gm_TimerStart (WORK1_10MS_TMR,W_TimeOut);

   while ((B_Status & 0x01) !=  0x00)
   {
      if (gm_TimerCheck(WORK1_10MS_TMR) != TIMER_OK)
      {
         msgx_i("SSP2",0);
         return   FALSE;
      }
      if (pChip->Driver->ReadStatusRegister(&B_Status)!=TRUE)
      {
         msgx_i("SSP3",0);
         return FALSE; // error SPI access
      }
   }

   return TRUE; // successful; polling OK
}

//******************************************************************************
//
// FUNCTION : BOOL SpiWriteProtectionDisable(void)
// USAGE    :
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiWriteProtectionDisable(void)
{
   BOOL Bl_Error = TRUE;//successful
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   SpiCmdStart();
//	Bl_Error = (SpiWrite(pChip->Commands->EWSR)); //error
   Bl_Error = (SpiWrite(pChip->Commands->WREN)); //error
   SpiCmdStop();
   if (Bl_Error==FALSE) return FALSE;
   SpiCmdStop(); //delay

   /*
      	if(SpiWriteEnable())
      		return FALSE;//error
   */

   SpiCmdStart();
   if (SpiWrite(pChip->Commands->WRSR)!=TRUE)
      Bl_Error = FALSE;//error
   else if (SpiWrite(0)!=TRUE)
      Bl_Error = FALSE;//error

   SpiCmdStop();
   return Bl_Error;
}
//******************************************************************************
//
// FUNCTION : BOOL SpiWriteProtectionEnable(void)
// USAGE    :
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiWriteProtectionEnable(void)
{
   BOOL Bl_Error = TRUE;//successful
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   SpiCmdStart();
//	Bl_Error = (SpiWrite(pChip->Commands->EWSR)); //error
   Bl_Error = (SpiWrite(pChip->Commands->WREN)); //error
   SpiCmdStop();

   if (Bl_Error==FALSE) return FALSE;
   SpiCmdStop(); //delay

   SpiCmdStart();
   if (SpiWrite(pChip->Commands->WRSR)!=TRUE)
      Bl_Error = FALSE;//error
   else if (SpiWrite(0x9c)!=TRUE)
      Bl_Error = FALSE;//error
   SpiCmdStop();

   return Bl_Error;
}

//******************************************************************************
// FUNCTION : BOOL SpiWriteEnable(void)
// USAGE    :
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiWriteEnable(void)
{
   BOOL Bl_Error; //successful
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   SpiCmdStart();
   Bl_Error = (SpiWrite(pChip->Commands->WREN)); //error
   SpiCmdStop();
   return Bl_Error;
}

//******************************************************************************
//
// FUNCTION : BOOL SpiWriteStatusRegister(BYTE B_Data)
// USAGE    :
// INPUT    : Register Value
// OUTPUT   : None
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
/* This function is not used in current driver release
static BOOL SpiWriteStatusRegister(BYTE B_Data)
{
	BOOL Bl_Error = TRUE;// successful;

	SpiCmdStart();
	if(SpiWrite(SPI_WRSR)) Bl_Error = FALSE;
	else if(SpiWrite(B_Data)) Bl_Error = FALSE;
	SpiCmdStop();
	return Bl_Error;
}
*/

//******************************************************************************
// FUNCTION : BOOL SpiPageProgram(DWORD Dw_Addr, BYTE * pB_Data, WORD W_Length)
// USAGE    : Page programming at the giving address
// INPUT    : Address, buffer pointer and value
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//******************************************************************************
static BOOL SpiPageProgram(DWORD Dw_Addr, BYTE * pB_Data, WORD W_Length)
{
   BOOL Bl_Error = TRUE;// successful;
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT) != TRUE)
   {
      msgx_i("PP1",0);
      return FALSE; //  execute poling
   }
   if (pChip->Driver->WriteEnable()!= TRUE)
   {
      msgx_i("PP2",0);
      return FALSE;
   }

   SpiCmdStart();
   if (SpiWrite(pChip->Commands->BYTE_PROGRAM)!= TRUE)
   {
      msgx_i("PP3",0);
      Bl_Error = FALSE;
   }
   else if (SpiWrite((BYTE) (Dw_Addr >> 16))!= TRUE)
   {
      msgx_i("PP4",0);
      Bl_Error = FALSE;
   }
   else if (SpiWrite((BYTE) (Dw_Addr >> 8))!= TRUE)
   {
      msgx_i("PP5",0);
      Bl_Error = FALSE;
   }
   else if (SpiWrite((BYTE) Dw_Addr)!= TRUE)
   {
      msgx_i("PP6",0);
      Bl_Error = FALSE;
   }
   else
      for (;W_Length--;)
      {
         if (SpiWrite(*pB_Data++)!= TRUE)
         {
            msgx_i("PP7",0);
            Bl_Error = FALSE;
            break;
         }
      }
   SpiCmdStop();

   return Bl_Error;
}

//******************************************************************************
// FUNCTION : BOOL SpiProgram(DWORD Dw_Addr, BYTE B_Data)
// USAGE    : Single byte programming at the giving address
// INPUT    : Byte address and value
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
// IMPORTANT NOTES: Execute the function SpiStatusPoll before using this function
//
//******************************************************************************
/* static BOOL SpiProgram(DWORD Dw_Addr, BYTE B_Data)
{
	BOOL Bl_Error = TRUE;// successful;

	if (SpiStatusPoll(WRITE_TIMEOUT) != TRUE) return FALSE; //  execute poling
	if (SpiWriteEnable()!=TRUE) return FALSE;

	SpiCmdStart();
	if(SpiWrite(SPI_BYTE_PROGRAM)!=TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) (Dw_Addr >> 16))!=TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) (Dw_Addr >> 8))!=TRUE) Bl_Error = FALSE;
	else if(SpiWrite((BYTE) Dw_Addr)!=TRUE) Bl_Error = FALSE;
	else if(SpiWrite(B_Data)!=TRUE) Bl_Error = FALSE;
	SpiCmdStop();

	return Bl_Error;
}
//******************************************************************************/
//
// FUNCTION : BOOL SpiChipErase(void)
// USAGE    : Erase SPI memory chip
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiChipErase(void)
{
   BOOL Bl_Error = TRUE;// successful;
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   msg("SpiChipErase");
   if (pChip->Driver->WriteEnable()!=TRUE) return FALSE;//error

   SpiCmdStart();
   Bl_Error = SpiWrite(pChip->Commands->ERASE);
   SpiCmdStop();

   return Bl_Error;
}

//******************************************************************************/
//
// FUNCTION : BOOL SpiSectorErase(DWORD Dw_Addr)
// USAGE    : Erase SPI secotr memory 
// INPUT    : DWORD Dw_Addr
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiSectorErase(DWORD Dw_Addr)
{
   BOOL Bl_Error;
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   msg_i("SectEr: 0x%lX",Dw_Addr);
   
   if (pChip->Driver->WriteEnable()!=TRUE) 
   {
      msgx("SSE0");
      return FALSE;//error      
   }
   if (pChip->Driver->StatusPoll(SECT_ERASE_TIMEOUT) != TRUE)
   {
      msgx("SSE1");
      return FALSE; //  execute poling
   }
   Bl_Error = TRUE;
   SpiCmdStart();
   if(SpiWrite(pChip->Commands->SECT_ERASE)!=TRUE) Bl_Error = FALSE;
   else if(SpiWrite((BYTE) (Dw_Addr >> 16))!=TRUE) Bl_Error = FALSE;
   else if(SpiWrite((BYTE) (Dw_Addr >> 8 ))!=TRUE) Bl_Error = FALSE;
   else if(SpiWrite((BYTE) Dw_Addr)!=TRUE) Bl_Error = FALSE;;
   SpiCmdStop();
   if(Bl_Error !=TRUE)
   {
      msgx_i("SSE2(0x%lX)",Dw_Addr);
      return FALSE;
   }
   if (pChip->Driver->StatusPoll(SECT_ERASE_TIMEOUT) != TRUE)
   {
      msgx("SSE2");
      return FALSE; //  execute poling
   }
   return TRUE;
}



//******************************************************************************
//
// FUNCTION : BOOL SpiReadBurst(DWORD Dw_StartAddr, DWORD Dw_StopAddr, BYTE * Ba_Buf)
// USAGE    :
// INPUT    : None
// OUTPUT   : None
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiReadBurst(DWORD Dw_StartAddr,BYTE B_Length, BYTE * Ba_Buf)
{
   BOOL Bl_Error = TRUE;// successful;
   ChipFeatures_t * pChip = SpiGetChipFeatures();

   BYTE B_Ix;
   // This is a self-contained flash device command to be used
   // after programming.  It is terminated at any arbitrary byte
   // in the read burst by issuing SpiCmdStop().
   //
   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT)== TRUE)
   {
      SpiCmdStart();
      if (SpiWrite(pChip->Commands->HS_READ)!=TRUE) Bl_Error = FALSE;
      else if (SpiWrite((BYTE) (Dw_StartAddr >> 16))!=TRUE) Bl_Error = FALSE;	// Transfer most significant address byte first (3 byte address)
      else if (SpiWrite((BYTE) (Dw_StartAddr >> 8))!=TRUE) Bl_Error = FALSE;
      else if (SpiWrite((BYTE) Dw_StartAddr)!=TRUE) Bl_Error = FALSE;
      else if (SpiWrite(0x00)!=TRUE) Bl_Error = FALSE;// dummi byte
      else
      {
         for (B_Ix=0; B_Ix<B_Length;B_Ix++)
         {
            if (SpiRead(Ba_Buf++)!=TRUE)
            {
               Bl_Error = FALSE;
               break;
            }
         }
      }
      SpiCmdStop();
   }
   else Bl_Error = FALSE;//error if polling failed

   return Bl_Error;
}


//******************************************************************************
//
// FUNCTION : BOOL SpiCheckIdMethod1(BYTE *Bp_Id)
// USAGE    : Check Flash VendorID and DeviceID
// INPUT    : ID
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiCheckIdMethod1(BYTE *Bp_Id)
{
   BOOL b_Er = FALSE;
   SpiCmdStart();
   if (SpiWrite (SPI_READ_ID_1))
   {
      if (SpiWrite(0x00))
      {
         if (SpiWrite(0x00))
         {
            if (SpiWrite(0x00))
            {
               if (SpiRead(Bp_Id))
               {
                  if ((*Bp_Id != 0)&&(*Bp_Id != 0xff))
                  {
                     if (SpiRead(++Bp_Id))
                     {
                        if ((*Bp_Id != 0)&&(*Bp_Id != 0xff))
                        {
                           b_Er = TRUE;	//success
                        }
                     }
                  }
               }
            }
         }
      }
   }
   SpiCmdStop();
   return b_Er;
}

//******************************************************************************
//
// FUNCTION : BOOL SpiCheckIdMethod2(BYTE *Bp_Id)
// USAGE    : Check Flash VendorID and DeviceID
// INPUT    : ID
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiCheckIdMethod2(BYTE *Bp_Id)
{
   BOOL b_Er = FALSE;
   SpiCmdStart();
   if (SpiWrite (SPI_READ_ID_2))
   {
      if (SpiRead(Bp_Id))
      {
         if ((*Bp_Id != 0)&&(*Bp_Id != 0xff))
         {
            if (SpiRead(++Bp_Id))
            {
               if ((*Bp_Id != 0)&&(*Bp_Id != 0xff))
               {
                  b_Er = TRUE;	//success
               }
            }
         }
      }

   }
   SpiCmdStop();
   return b_Er;
}

//******************************************************************************
//
// FUNCTION : static BOOL SpiCheckId(BYTE * Bp_Id);
// USAGE    : Read and verify ID
// INPUT    : A pointer to ID buffer (two bytes desired)
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
static BOOL SpiCheckId(BYTE * Bp_Id)
{
   BOOL b_Er = FALSE;

   b_Er = SpiCheckIdMethod1(Bp_Id);
   if (b_Er==FALSE)
   {
      b_Er = SpiCheckIdMethod2(Bp_Id);
   }

   return b_Er;
}

//******************************************************************************
//
// FUNCTION : pChipFeatures_t SpiGetChipFeatures(void)
// USAGE    : SpiGetChipFeatures
// INPUT    : None
// OUTPUT   : pChipFeatures_t
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
pChipFeatures_t SpiGetChipFeatures(void)
{
   static ChipFeatures_t ChipFeatures;

   return ((pChipFeatures_t) &ChipFeatures);
}


//******************************************************************************
//
// FUNCTION : BOOL SpiSetChipFeatures(void)
// USAGE    : SpiSetChipFeatures
// INPUT    : pChipFeatures_t
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
BOOL SpiSetChipFeatures(void)
{

   ChipFeatures_t * p_ChipFeatures;
   WORD W_Ix;
   BYTE Bp_Id[2] = {0,0};

   p_ChipFeatures = SpiGetChipFeatures();

   if (SpiCheckId(Bp_Id)==FALSE) return FALSE;


   // Check if Chip is
   for (W_Ix = 0; (ChipFeaturesTable[W_Ix].B_MID !=0)&&(ChipFeaturesTable[W_Ix].B_DID !=0);W_Ix++)
   {
      if ( (ChipFeaturesTable[W_Ix].B_MID == Bp_Id[0]) && (ChipFeaturesTable[W_Ix].B_DID == Bp_Id [1]))
         break;
   }

   p_ChipFeatures->B_MID = Bp_Id[0];
   p_ChipFeatures->B_DID = Bp_Id[1];
   p_ChipFeatures->DW_Size = ChipFeaturesTable[W_Ix].DW_Size;
   p_ChipFeatures->W_Page = ChipFeaturesTable[W_Ix].W_Page;
   p_ChipFeatures->W_SectorSize= ChipFeaturesTable[W_Ix].W_SectorSize;   
   p_ChipFeatures->Driver = ChipFeaturesTable[W_Ix].Driver;
   p_ChipFeatures->Commands= ChipFeaturesTable[W_Ix].Commands;

   msg_ii("Chip Vender ID: 0x%X    Device ID: 0x%X",Bp_Id[0], Bp_Id[1]);

   return TRUE;
}



//******************************************************************************
//
// FUNCTION : BOOL EnableWriteProtection(void)
// USAGE    : EnableWriteProtection
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
BOOL EnableWriteProtection(void)
{
   pChipFeatures_t pChip = SpiGetChipFeatures();
   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT) != TRUE)
   {
      return FALSE;
   }
   return pChip->Driver->WriteProtectionEnable();
}


//******************************************************************************
//
// FUNCTION : BOOL ClearWriteProtection(void)
// USAGE    : ClearWriteProtection
// INPUT    : None
// OUTPUT   : TRUE if successful, FALSE if ERROR
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
BOOL ClearWriteProtection(void)
{
   pChipFeatures_t pChip = SpiGetChipFeatures();
   if (pChip->Driver->StatusPoll(WRITE_TIMEOUT) != TRUE)
   {
      return FALSE;
   }
   return pChip->Driver->WriteProtectionDisable();
}


//******************************************************************************
//
// FUNCTION : void GetSpiFlashInfo(FlashInfo_t * pInfo)
// USAGE    : GetSpiFlashInfo
// INPUT    : FlashInfo_t
// OUTPUT   : None
// GLOBALS  : None
// USED_REGS: None
//
//******************************************************************************
void GetSpiFlashInfo(FlashInfo_t * pInfo)
{
   pChipFeatures_t pChip = SpiGetChipFeatures();
   pInfo->B_MID       = pChip->B_MID;
   pInfo->B_DID       = pChip->B_DID;
   pInfo->DW_Size     = pChip->DW_Size;
   pInfo->W_Page      = pChip->W_Page;   
   pInfo->W_SectorSize= pChip->W_SectorSize;   
}

#endif   // SPIFLASH_CTRL_DRV

