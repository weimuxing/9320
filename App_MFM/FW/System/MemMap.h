/*
   $Workfile:   MemMap.h  $
   $Revision: 1.13 $
   $Date: 2012/04/10 10:01:26 $
*/
#ifndef __MEMMAP_H__
#define __MEMMAP_H__
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
// MODULE:     Memmap.h
//
// USAGE:      File containing memory map calculation  and definitions
//
//******************************************************************************
#define DRAM_CACHE_BURST_SIZE 64  //DRAM cache burst size CACHE_CTRL3:CACHE_BURST_SIZE_DRAM (64 or 128)

//******************************************************************************
//  M E M O R Y    M A P
//******************************************************************************
// Here is only calculation. Should not be modified!
#define DATA_DRAM_SIZE (64 * 1024L)   //when changed appropriate changes in CFG_FILE_... required.

//---------------------------------------------
#ifndef __ADDR24__ //20bit (real) address mode 
#error Application does not support 20bit address mode
#else //24 bit (extended address mode)------------------------------------------

#if (defined(ROM_SIZE_1MB))
#define CODE_START_ADDRESS 0x410000U
#define CFG_FILE_CODE_MAP_DECR 0x40FFFFU //used in cfg-file    
#define CFG_FILE_CODE_CLASS 0x4100
#define DATA_DRAM_ADDR 0x400000U //(CODE_START_ADDRESS-DATA_DRAM_SIZE)
#define CFG_FILE_RESERVED_END 0x3FFFFFU //used in cfg-file
#define CFG_FILE_DATA_CLASS 0x4000 //used in cfg-file        
#define CODE_SIZE 0x0F0000U
#define CODE_END_ADDRESS 0x4FFFFFU
#define CFG_FILE_CODE_END_CLASS 0x4FFF
#define CFG_FILE_BINARY_SIZE 1024
#elif (defined(ROM_SIZE_2MB))
#define CODE_START_ADDRESS 0x410000U
#define CFG_FILE_CODE_MAP_DECR 0x40FFFFU //used in cfg-file    
#define CFG_FILE_CODE_CLASS 0x4100
#define DATA_DRAM_ADDR 0x400000U //(CODE_START_ADDRESS-DATA_DRAM_SIZE)
#define CFG_FILE_RESERVED_END 0x3FFFFFU //used in cfg-file
#define CFG_FILE_DATA_CLASS 0x4000 //used in cfg-file        
#define CODE_SIZE 0x1F0000U
#define CODE_END_ADDRESS 0x5FFFFFU
#define CFG_FILE_CODE_END_CLASS 0x5FFF
#define CFG_FILE_BINARY_SIZE 2048
#elif (defined(ROM_SIZE_4MB))
#define CODE_START_ADDRESS 0x410000U
#define CFG_FILE_CODE_MAP_DECR 0x40FFFFU //used in cfg-file    
#define CFG_FILE_CODE_CLASS 0x4100
#define DATA_DRAM_ADDR 0x400000U //(CODE_START_ADDRESS-DATA_DRAM_SIZE)
#define CFG_FILE_RESERVED_END 0x3FFFFFU //used in cfg-file
#define CFG_FILE_DATA_CLASS 0x4000 //used in cfg-file        
#define CODE_SIZE 0x3F0000U
#define CODE_END_ADDRESS 0x7FFFFFU
#define CFG_FILE_CODE_END_CLASS 0x7FFF
#define CFG_FILE_BINARY_SIZE 4096
#elif (defined(ROM_SIZE_8MB))
#define CODE_START_ADDRESS 0x410000U
#define CFG_FILE_CODE_MAP_DECR 0x40FFFFU    //used in cfg-file                
#define CFG_FILE_CODE_CLASS 0x4100
#define DATA_DRAM_ADDR 0x400000U //(CODE_START_ADDRESS-DATA_DRAM_SIZE)
#define CFG_FILE_RESERVED_END 0x3FFFFFU //used in cfg-file
#define CFG_FILE_DATA_CLASS 0x4000 //used in cfg-file        
#define CODE_SIZE 0x7F0000U
#define CODE_END_ADDRESS 0xBFFFFFU
#define CFG_FILE_CODE_END_CLASS 0xBFFF
#define CFG_FILE_BINARY_SIZE 8192
#elif (defined(ROM_SIZE_512K))
#define CODE_START_ADDRESS 0x410000U
#define CFG_FILE_CODE_MAP_DECR 0x40FFFFU //used in cfg-file    
#define CFG_FILE_CODE_CLASS 0x4100
#define DATA_DRAM_ADDR 0x400000U //(CODE_START_ADDRESS-DATA_DRAM_SIZE)
#define CFG_FILE_RESERVED_END 0x3FFFFFU //used in cfg-file
#define CFG_FILE_DATA_CLASS 0x4000 //used in cfg-file            
#define CODE_SIZE 0x070000U
#define CODE_END_ADDRESS 0x47FFFFU
#define CFG_FILE_CODE_END_CLASS 0x47FF
#define CFG_FILE_BINARY_SIZE 512
#else
#error Application does not support other than 1/2/4/8 MB flash size in 24bit address mode
#endif // (defined(ROM_SIZE_1MB))

#endif // ndef __ADDR24__

#endif //MEMMAP
//*********************************  END  **************************************

