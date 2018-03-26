/*
    $Workfile:   soft_I2C.C  $
    $Revision: 1.3 $
    $Date: 2011/12/15 03:52:11 $
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
// MODULE:      soft_i2c.c
//
// USAGE:       This module contains api's for software i2c
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "System\All.h"


// 20090730 #1, Implement DP EDID wirte by SW I2C.
#ifdef SOFT_I2C_FOR_EDID

#ifndef SOFT_I2C_ACCESS

#define SOFT_I2C_ACCESS

#if 1//def MSG_PRINT_SOFTI2C
#define	msgPrint_softi2c  gm_Print
#else
#define msgPrint_softi2c(a,b)
#endif
#if 1//def USE_SOFT_I2C//071213 Edward

//#define I2C_DONT_CARE 0xFF
#define SOFTWARE_I2C_ACCESS_ENABLE

#if 1
#define SDA_PIN	GPIO93_IO
#define SCL_PIN	GPIO94_IO
#else
#define SDA_PIN	GPIO94_IO
#define SCL_PIN	GPIO93_IO
#endif


//********** I2c
/////////DONT CHANGETHE FOLLOWING MACROS //////////////////////////////
//SET_CLK selects the GPIO in OUTPUT mode and writes 1

#define BUS1_SET_SCLK();  { gm_SetRegBitsWord(GPOUTPUT5, SCL_PIN); \
                                            gm_SetRegBitsWord(GPIO_5_DIRCTRL, SCL_PIN); \
}
//CLR_SCLK selects the GPIO in OUTPUT mode and writes 0

#define BUS1_CLR_SCLK();  { gm_SetRegBitsWord(GPIO_5_DIRCTRL, SCL_PIN); \
                                             gm_ClearRegBitsWord(GPOUTPUT5, SCL_PIN); \
}
//BUS1_SET_SDATA selects the GPIO in OUTPUT mode and writes 1
#define BUS1_SET_SDATA(); { gm_SetRegBitsWord(GPIO_5_DIRCTRL, SDA_PIN); \
	                                        gm_SetRegBitsWord(GPOUTPUT5, SDA_PIN); \
}
//BUS1_CLR_SDATA selects the GPIO in INPUT mode and writes 0
#define BUS1_CLR_SDATA(); { gm_SetRegBitsWord(GPIO_5_DIRCTRL, SDA_PIN); \
                                               gm_ClearRegBitsWord(GPOUTPUT5, SDA_PIN);\
}


//GPIO_INPUT_DIR

#define BUS1_CONFIG_FOR_SDATA_READ() gm_ClearRegBitsWord(GPIO_5_DIRCTRL, SDA_PIN)

#define BUS1_CONFIG_FOR_SCLK_READ()  gm_ClearRegBitsWord(GPIO_5_DIRCTRL, SCL_PIN)


#define BUS1_CONFIG_FOR_GET_ACK()   gm_ClearRegBitsWord(GPIO_5_DIRCTRL, SDA_PIN)

#define BUS1_READ_SDATA() (gm_ReadRegWord(GPINPUT5) & SDA_PIN)
#define BUS1_READ_SCLK()  (gm_ReadRegWord(GPINPUT5) & SCL_PIN)


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S

///// Local  Functions
static void gm_i2c_BitDly(void);
static void Clock_High_Check();
extern void I2C_BUS1_START();
extern void I2C_BUS1_SEND_ACK();
static void I2C_BUS1_ACK();
extern void I2C_BUS1_STOP();
static void I2C_START();
static void I2C_SEND_ACK();
static void I2C_STOP();
extern void Clock_Low();
extern void Clock_High();
static void Clock_High_Check();
extern void Data_Low();
extern void Data_High();
static BYTE Data_Check();
extern void I2C_Send_Data (BYTE I2COutData);
static void I2C_Option_Check(BYTE bus, BYTE chip_id, WORD addr);
static void I2C_Option_Clear();
static void I2C_255Err_Print(BYTE cErrCount);
static void I2C_Debug_Print1(void);


//******************************************************************************
//
// FUNCTION:    void gm_i2c_BitDly(void)
// USAGE:       Ensure minimum high & low clock for I2C bus,
//              fine tuned for oscillator frequency used
//
// INPUT:       None
// OUTPUT:      None
// GLOBALS:		None
// USED_REGS:	None
//
//******************************************************************************
void gm_i2c_BitDly(void)
{
   // to generate required clock cycles for 4.7 microsec delay (20 Mhz clock)

   WORD count;
   count = 50;

   while (count)
   {
      _asm
      {
         nop;
      }
      count--;
   }
   //			gm_Delay1ms(1);

/// funcion call instruction  takes 31 clock cycles
/// ret  instruction takes   30 clock cycles
/// push cs instruction takes 13 clock cycles
/// pop cs instruction takes 12 clock cycles
/// Total  clock cycles =  98 clock cycles
//// Total time at 20MHz internal clock = 0.05us * 98 = 4.9us

//NOTE 1601 uses 100 Mhz clock
// 1 clock delay = 0.01 us
// Total clock delay required for 4.7 us delay is 470 clocks
// The function call and its associated push pop will give a delay of 86 clocks
// Additional 384 clock delay is required. == 128 NOP;

//NOTE currently the wait state for XROM is 16 clock cycles if this is changed,
// probably more NOP should be added accordingly
}

////////////////////////////////////////////////////////////
WORD	I2C_Flag;
BYTE	I2C_Bus;
BYTE	I2C_Dev_ID;
BYTE	I2C_Debug_On;
BYTE	g_is_I2C_Wait2Long;
BYTE	g_is_I2C_error;

typedef enum
{
   I2C_FLAG_EEPROM				= 0x0001,		// When using i2c for EEPROM
   I2C_FLAG_NOSUBADDR			= 0x0002	,		// for No Sub Addr I2C
   I2C_FLAG_SUBADDR_1                   = 0x0004,
   I2C_FLAG_SUBADDR_2                      = 0x0008,
   I2C_FLAG_MSP_CHIP_ID_WAIT	= 0x0010,			// for MSP Chip Wait State
   I2C_FLAG_DDP1010_WAIT	= 0x0020			// for DDP1010
} eI2C_FLAG_;


#define I2C_DELAY 	gm_i2c_BitDly()
//#define I2C_DONT_CARE			(WORD)0xFFFF

void Clock_High_Check(void)
{
   BYTE Count=255;

   //BUS1_SET_SCLK();
   //I2C_DELAY;
   //I2C_DELAY;
   I2C_DELAY;
   BUS1_CONFIG_FOR_SCLK_READ();  //set the SCL to the input  ,because have the pull high resistance,so the SCL should be high
   while ((!BUS1_READ_SCLK()) && Count)
   {
      I2C_DELAY;   //if SCL is low ,start count to wait to detect all the count timer.but if the SCL is high go the next setp
      Count--;
   }

   if (Count == 0)
   {
      g_is_I2C_Wait2Long = 1;   //when delay the 255*4.7ms ,the SCL is still low ,set the g_is_I2C_Wait2Long = 1;
   }
   else
   {
      g_is_I2C_Wait2Long = 0;   //the SCL is high set the g_is_I2C_Wait2Long = 0;
   }
}



/*	I2C 1	*/
void I2C_BUS1_START(void)
{
   BUS1_SET_SDATA();
   BUS1_SET_SCLK();
   //
   I2C_DELAY;
   I2C_DELAY;
   BUS1_CLR_SDATA();
   I2C_DELAY;
   I2C_DELAY;
   BUS1_CLR_SCLK();
   I2C_DELAY;
}

void I2C_BUS1_SEND_ACK(void)
{
   BUS1_CLR_SDATA();
   I2C_DELAY;
   BUS1_SET_SCLK();
   I2C_DELAY;
   BUS1_CLR_SCLK();
}

void 	I2C_BUS1_ACK(void)
{
   BYTE	Count = 255;

   BUS1_CLR_SCLK(); //set SCL low
   //BUS1_SET_SDATA();
   BUS1_CONFIG_FOR_GET_ACK();//set SDA turn to Input
   I2C_DELAY;
   Clock_High_Check();//SCL pull high
   I2C_DELAY;
   /*
   //BUS1_READ_SDATA();
   while (BUS1_READ_SDATA() && Count) {I2C_DELAY; Count--;} // if read the SDA is low (have ACK signal)go the next setp ,if not start count to delay to read the SDA.. (wait for ACK)
   BUS1_CLR_SCLK();  //set SCL low
   //I2C_DELAY;
   if(BUS1_READ_SDATA()) g_is_I2C_error = 1; //if no ack set  g_is_I2C_error = 1; have ACK set g_is_I2C_error =0;
   else g_is_I2C_error = 0;
   //BUS1_CLR_SCLK();  //set SCL low
   */
   I2C_DELAY;
   while (BUS1_READ_SDATA() && Count)
   {
      I2C_DELAY;
      Count--;

   } // if read the SDA is low (have ACK signal)go the next setp ,if not start count to delay to read the SDA.. (wait for ACK)
   BUS1_CLR_SCLK();  //set SCL low
   //I2C_DELAY;
   if (Count == 0)
   {
      g_is_I2C_error = 1; //if no ack set  g_is_I2C_error = 1; have ACK set g_is_I2C_error =0;
      //	gm_Print(">>>>>>>>>>>>>no ack>>>>>>>>>>>",0);
   }
   else
      g_is_I2C_error = 0;
}

void 	I2C_BUS1_STOP(void)
{
//	BUS1_CLR_SCLK();
   BUS1_CLR_SDATA();
   Clock_High_Check();
   I2C_DELAY;
   BUS1_SET_SDATA();
   I2C_DELAY;
}

void far far_i2c_SendStop(void)
{
   I2C_BUS1_STOP();
}

void I2C_START(void)
{
   I2C_BUS1_START();
}

void I2C_SEND_ACK(void)
{
   I2C_BUS1_SEND_ACK();
}

void SoftI2C_ACK(void)
{
   I2C_BUS1_ACK();
}

void I2C_STOP(void)
{
   I2C_BUS1_STOP();
}

void Clock_Low(void)
{
   BUS1_CLR_SCLK();
}

void Clock_High(void)
{
   BUS1_SET_SCLK();
}


void Data_Low(void)
{
   BUS1_CLR_SDATA();
}
void Data_High(void)
{
   BUS1_SET_SDATA();
}

BYTE Data_Check(void)
{
   BUS1_CONFIG_FOR_SDATA_READ();  //set SDA to input
   return ((BUS1_READ_SDATA())?1:0); // read SDA is high return 1,else return 0
}

void I2C_Send_Data (BYTE I2COutData)
{
   BYTE bitmask = 0x80;

   while ( bitmask )
   {
      Clock_Low(); //set clock low
      if ( I2COutData & bitmask )
      {
         Data_High();
      }
      else
      {
         Data_Low();
      }
      I2C_DELAY;

      if (bitmask == 0x80)
      {
         Clock_High_Check();
      }
      else
      {
         Clock_High();
      }


      //	Clock_High();

      bitmask = bitmask >> 1;
      I2C_DELAY;
   }

   SoftI2C_ACK();
}

void I2C_Option_Check(BYTE bus, BYTE chip_id, WORD addr)
{
   I2C_Flag =0;
   if (addr == I2C_DONT_CARE)
   {
      I2C_Flag |= I2C_FLAG_NOSUBADDR;
   }
   else if (addr <= 0x00FF)
      I2C_Flag |= I2C_FLAG_EEPROM;
   else
      I2C_Flag |= I2C_FLAG_SUBADDR_2;

   I2C_Bus = bus;
   I2C_Dev_ID = chip_id;
}

void I2C_Option_Clear(void)
{
   I2C_Flag = 0;
   I2C_Bus = 0;
   I2C_Dev_ID = 0;
}

void I2C_255Err_Print(BYTE cErrCount)
{
   if (cErrCount == 255)
   {
      msgPrint_softi2c("devid(%x) _ I2C ERROR after 255 retry!!!",I2C_Dev_ID);
   }
   I2C_Option_Clear();
}

void I2C_Debug_Print1(void)
{
   if (g_is_I2C_error)
   {
//		msgPrint_softi2c("devid(%d) _ I2C No Ack",I2C_Dev_ID);
   }
   if (g_is_I2C_Wait2Long)
   {
      msgPrint_softi2c("devid(%d) _ I2C Wait to Long",I2C_Dev_ID);
   }
}

BYTE I2C_Burst_Read_Chartest(void)
{
   I2C_START();
   I2C_START();
   I2C_START();
   return TRUE;
}
BYTE far I2C_Burst_Read_Char(BYTE bus, BYTE chip_id, WORD address, BYTE far* pData, WORD wNumOfBytes)
{
   BYTE	i,cErrCount,errstep,
   read_data,
   bitmask;
   WORD cnt;



   // Adjust Number of bytes to write
   if (wNumOfBytes == 0)
   {
      wNumOfBytes = (WORD)65535L;
   }
   else
   {
      wNumOfBytes--;
   }

   I2C_Option_Clear();

   chip_id &= 0xfe;
   I2C_Option_Check(bus, chip_id,address);

   for (cErrCount=0;cErrCount<=254;cErrCount++)
   {
      read_data = 0;
      bitmask = 0x80;
      // Send Start
      I2C_START();
      if (!(I2C_Flag & I2C_FLAG_NOSUBADDR))
      {
         // Send Dummy ID address with a write.
         I2C_Send_Data( chip_id );
         if (g_is_I2C_error)
         {
            errstep =1;
            goto NoAck_Error;
         }
         if (I2C_Flag & I2C_FLAG_SUBADDR_2) // word address  //start==>chipid==>address high 8bit,low 8bit==>stop ==>start==>chipid+1==>read DATA
         {
            I2C_Send_Data((BYTE)(address>>8));
            if (g_is_I2C_error) goto NoAck_Error;
            I2C_Send_Data((BYTE)address);
            if (g_is_I2C_error) goto NoAck_Error;
            I2C_STOP();
         }
         else  //byte address
         {
            // Send LSB Address
            I2C_Send_Data( (BYTE)address);
            if (g_is_I2C_error)
            {
               errstep =2;
               goto NoAck_Error;
            }
         }
         // Repeat Start
         I2C_START();
      }
      // Send chip ID address with  read bit.
      I2C_Send_Data( chip_id | 0x1 ); // I2C ID value, write is high     //start==>ChipID+1==>read DATA
      if (g_is_I2C_error)
      {
         errstep =3;
         goto NoAck_Error;
      }
      // Read I2C DATA
      //   read_data = 0;
      for (cnt = 0; cnt <= wNumOfBytes; cnt++)
      {
         read_data = 0;
         bitmask = 0x80;
         //Data_High();
         BUS1_CONFIG_FOR_SDATA_READ(); //config SDA to input, equal to pull high
         for ( i = 0; i < 8; i++ )
         {
            I2C_DELAY;	//honggari_4Apr : added
            if (i)
            {
               Clock_High();
            }
            else
            {
               Clock_High_Check();//config SCL to input ,equal to pull high
               if (g_is_I2C_Wait2Long)
               {
                  errstep = 4;
                  goto NoAck_Error;
               }
            }

            I2C_DELAY;
            if (Data_Check())
            { // need false read to fool compiler optimization.
               read_data = read_data | bitmask;
            }
            I2C_DELAY;	//honggari_4Apr : added
            Clock_Low();
            bitmask = bitmask >> 1;
         }
         if (cnt != wNumOfBytes)
         {
            I2C_SEND_ACK();
            *pData = read_data;
            pData++;
         }
         else
         {
            // Non Acknowledge Cycle
//				Clock_High();
//				I2C_DELAY;
//				Clock_Low();
//				I2C_DELAY;
//				Clock_High();
            *pData = read_data;
         }
      }

NoAck_Error:
      // Send Stop
      I2C_STOP();
      //I2C_START();
      if (!(g_is_I2C_error|g_is_I2C_Wait2Long))
      {
         break;		//hongari_9May : I2C 1 Cycle compledte with No Error
      }
      else
      {
         I2C_Debug_Print1();
#ifdef ResetKeypadForESDTest//071224 Edward for ESD
         return 0x55;
#endif
      }
   }

   I2C_255Err_Print(cErrCount);

   if (errstep)
   {
//			msgPrint_softi2c(" errstep = %d",errstep);
   }

   return read_data;
}

void far I2C_Burst_Write_Char( BYTE bus, BYTE chip_id, WORD address, BYTE far* pData, WORD wNumOfBytes )
{
   BYTE cnt,cErrCount,c4BytesCounter=0;

   msgPrint_softi2c("I2C_Burst_Write_Char address = %x ",(WORD)address);

   // Adjust Number of bytes to write
   if (wNumOfBytes == 0)
   {
      wNumOfBytes = (WORD)65535L;
   }
   else
   {
      wNumOfBytes--;
   }
   I2C_Option_Clear();

   chip_id &= 0xfe;  //set the lowest bit to 0 to enable the write enable
   I2C_Option_Check(bus, chip_id,address);
   for (cErrCount = 0; cErrCount <= 254; cErrCount++)
   {
      I2C_START();
      I2C_Send_Data(chip_id );	//start==>chipID==>address high 8bit, low 8bit==>Numbers
      if (g_is_I2C_error)
      {
         goto NoAck_Error;
      }
      if (I2C_Flag & I2C_FLAG_SUBADDR_2)
      {
         I2C_Send_Data((BYTE)(address>>8));
         if (g_is_I2C_error)
            goto NoAck_Error;
         I2C_Send_Data((BYTE)address);
         if (g_is_I2C_error)
            goto NoAck_Error;
      }
      else if (!(I2C_Flag & I2C_FLAG_NOSUBADDR))
      {	//honggari_8May : In case "not" NoSubAddr
         I2C_Send_Data((BYTE)address);
         if (g_is_I2C_error)
            goto NoAck_Error;
         if (I2C_Flag & I2C_FLAG_DDP1010_WAIT)
         {
            c4BytesCounter++;
         }
      }
      for (cnt = 0; cnt <= wNumOfBytes; cnt++)
      {
         I2C_Send_Data(*pData);
         if (g_is_I2C_error)
            goto NoAck_Error;
         if (I2C_Flag & I2C_FLAG_DDP1010_WAIT)
         {
            c4BytesCounter++;
            if (c4BytesCounter == 4)
            {
               //ddp1010 needs delay every 4 bytes send
               gm_Delay1ms(5);
               c4BytesCounter = 0;
            }
         }
         if (cnt != wNumOfBytes)
         {
            pData++;
         }
      }
NoAck_Error:
      // Send Stop
      I2C_STOP();
      I2C_DELAY;

      //I2C_START();
      if (!(g_is_I2C_error|g_is_I2C_Wait2Long))
      {
         break;		//hongari_9May : I2C 1 Cycle compledte with No Error
      }
      else
      {
         I2C_Debug_Print1();
      }
   }
   // start the count for next NVRAM access
   I2C_255Err_Print(cErrCount);
}


#endif // SOFTWARE_I2C_ACCESS_ENABLE

/*

// A Generic I2C Application Interface.
//******************************************************************************
// FUNCTION     :   void gm_utlWriteI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devRxData, BYTE rxDataLen)
// USAGE        :   Used to write thru i2c protocol
// INPUT        :	I2c Ring, devid, address_array, address len, data array, datalen
// OUTPUT       :   None
// GLOBALS      :	None
// USED_REGS    :	None
//******************************************************************************

gmt_RET_STAT gm_utlWriteI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devTxData, BYTE txDataLen)
{
	gmt_RET_STAT i2cRetvalue;
	WORD wSubAddr = 0;

	if (i2cRing == 0)
	{
        i2cRetvalue = gm_I2cWriteAddrBuffer(deviceID,&subAddr[0], subAddrLen,&devTxData[0], txDataLen);
	}
	else
	{
        #ifdef SOFTWARE_I2C_ACCESS_ENABLE

		if (subAddrLen == 0)
		{
			I2C_Burst_Write_Char(1, deviceID, I2C_DONT_CARE, &devTxData[0], txDataLen );
		}
		else if (subAddrLen == 1)
		{
		       wSubAddr = (WORD)subAddr[0];
			I2C_Burst_Write_Char(1, deviceID, wSubAddr, &devTxData[0], txDataLen );
		}
		else if (subAddrLen == 2)
		{
		       wSubAddr = (WORD)subAddr[0] << 8;
			wSubAddr |=  (WORD)subAddr[1];

			I2C_Burst_Write_Char(1, deviceID, wSubAddr, &devTxData[0], txDataLen );
		}
		else
		{
			msgPrint_softi2c("Second Ring-- Address Error ",0);
		}

		i2cRetvalue = OK;
        #endif // SOFTWARE_I2C_ACCESS_ENABLE
	}

	if (i2cRetvalue != OK)
	{
		msgPrint_softi2c("gm_I2cWriteAddrBuffer i2cRetValue for deviceid 0x%x = ",(WORD)deviceID);
		msgPrint_softi2c(" %d ",(WORD)i2cRetvalue);
	}
   return i2cRetvalue;
}


//******************************************************************************
// FUNCTION     :   void gm_utlReadI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devRxData, BYTE rxDataLen)
// USAGE        :   Used to read thru i2c protocol.
// INPUT        :	I2c Ring, devid, address_array, address len, data array, datalen
// OUTPUT       :   None
// GLOBALS      :	None
// USED_REGS    :	None
//******************************************************************************

gmt_RET_STAT gm_utlReadI2C(BYTE i2cRing, BYTE deviceID, BYTE *subAddr, BYTE subAddrLen, BYTE *devRxData, BYTE rxDataLen)
{
    BYTE *rxArray;
	gmt_RET_STAT i2cRetvalue;
     WORD wSubAddr =0;
    rxArray = devRxData;

	if (i2cRing == 0)
	{
        i2cRetvalue = gm_I2cReadAddrBuffer(deviceID,&subAddr[0],subAddrLen,(BYTE *)rxArray, rxDataLen);
    }
	else
	{
        #ifdef SOFTWARE_I2C_ACCESS_ENABLE
	if (subAddrLen == 0)
	{
	   		I2C_Burst_Read_Char(1, deviceID, I2C_DONT_CARE, (BYTE *)rxArray, rxDataLen );
    	}
	else if (subAddrLen == 1)
	{
	   	       wSubAddr = (WORD)subAddr[0];
	   		msgPrint_softi2c("Second Ring read0-- subAddrLen 1 ",0);
    		        I2C_Burst_Read_Char(1, deviceID, wSubAddr, (BYTE *)rxArray, rxDataLen );
    	}
	else if (subAddrLen == 2)
		{
		       wSubAddr = (WORD)subAddr[0] << 8;
			wSubAddr |=  (WORD)subAddr[1];

			I2C_Burst_Read_Char(1, deviceID, wSubAddr, (BYTE *)rxArray, rxDataLen  );
		}
    	else
    	{
    		msgPrint_softi2c("Second Ring read0 -- Address Error ",0);
    	}
   		i2cRetvalue = OK;
        #endif //SOFTWARE_I2C_ACCESS_ENABLE
   	}

    if (i2cRetvalue != OK)
    {
	    msgPrint_softi2c("gm_I2cReadAddrBuffer2 i2cRetValue for deviceid 0x%x = ",(WORD)deviceID);
	    msgPrint_softi2c(" %d ",(WORD)i2cRetvalue);
    }

    return i2cRetvalue;
}
*/
#endif
#endif
