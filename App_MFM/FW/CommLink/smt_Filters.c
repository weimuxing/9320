/*
   $Workfile:   smt_Filters.c  $
   $Revision: 1.7 $
   $Date: 2012/11/23 09:38:25 $
*/

//**********************************************************************
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
//======================================================
//
// MODULE:      smt_Filters.c
//
//**********************************************************************
// DOCUMENT: http://torpvcs/TorontoSoftwareDocs/Tools_FW_Interface/SMT/Filters_Tool/Filters_Tool_SMT_interface.doc Rev_1.2
// Document describes high level protocol between external tool and this message handler
//**********************************************************************

#include <mem.h>
#include "CommLink\smt.h"
#include "CommLink\smt_Filters.h"
#include "stdlibreplace\gm_UploadSRAMBlock.h"

#ifdef USE_SMT
// *

#ifdef DEBUG_MSG
#define  msg(a,b) //gm_Printf((const char far *)a,b)
#define  msgd(a,b,c) //gm_Printf((const char far *)a,b,c)
#else
#define msg(a,b)
#define msgd(a,b,c)
#endif

//*********************************************************
// LOCAL DEFINITIONS
//********************************************************
#define CR 1 // Filters supported by all Cortez chips
#if (FEATURE_SHARPNESS == REV6)
#define PL 1 // Filters supported by PLUS, ADVANCED and TORINO
#define AD 1 // Filters supported by  ADVANCED and TORINO
#endif

// definitioonts of table offsets of different filters
#define	MVF_UV_LPF_OF 		(AD == 1) ? 0x0000:0x0000				//05 - 1 Verti	0 MAIN	1 UV	00 LPF
#define	MVF_UV_HPF_OF	 	(AD == 1) ? 0x1800:0x0000				//0d - 1 Verti	0 MAIN	1 UV	01 HPF
#define	MVF_Y_LPF_OF 		(AD == 1) ? 0x0800:0x0800				//01 - 1 Verti	0 MAIN	0  Y	00 LPF
#define	MVF_Y_HPF_OF 		(AD == 1) ? 0x1000:0x0800				//09 - 1 Verti	0 MAIN	0  Y	01 HPF
#define	PVF_UV_LPF_OF 		(AD == 1) ? 0x0000:0					//07 - 1 Verti	1  PIP	1 UV	00 LPF
#define	PVF_UV_HPF_OF 		(AD == 1) ? 0x0300:0					//0f - 1 Verti	1  PIP	1 UV	01 HPF
#define	PVF_Y_LPF_OF 		(AD == 1) ? 0x0100:0					//03 - 1 Verti	1  PIP	0  Y	00 LPF
#define	PVF_Y_HPF_OF 		(AD == 1) ? 0x0200:0					//0b - 1 Verti	1  PIP	0  Y	01 HPF
#if (FEATURE_SHARPNESS == REV6)
#define	MHF_UV_LPF_OF	 	(AD == 1) ? 0x0000:0					//04 - 0 Horis	0 MAIN	1 UV	00 LPF
#define	MHF_UV_HPF_OF	 	(AD == 1) ? 0x0C00:0					//0c - 0 Horis	0 MAIN	1 UV	01 HPF
#define	MHF_Y_LPF_OF		(AD == 1) ? 0x0400:0					//00 - 0 Horis	0 MAIN	0  Y	00 LPF
#define	MHF_Y_HPF_OF 		(AD == 1) ? 0x0800:0					//08 - 0 Horis	0 MAIN	0  Y	01 HPF
#endif
#define	PHF_UV_LPF_OF 		(AD == 1) ? 0x0000:0					//06 - 0 Horis	1  PIP	1 UV	00 LPF
#define	PHF_UV_HPF_OF 		(AD == 1) ? 0x0C00:0					//0e - 0 Horis	1  PIP	1 UV	01 HPF
#define	PHF_Y_LPF_OF 		(AD == 1) ? 0x0400:0					//02 - 0 Horis	1  PIP	0  Y	00 LPF
#define	PHF_Y_HPF_OF 		(AD == 1) ? 0x0800:0					//0a - 0 Horis	1  PIP	0  Y	01 HPF

// definitions of data sizes of different filters (in BYTEs).
#define	MVF_UV_LPF_SZ 		(AD == 1) ? 1056 :1088 					//05 - 1 Verti	0 MAIN	1 UV	00 LPF
#define	MVF_UV_HPF_SZ	 	(AD == 1) ? 1056 :1088 					//0d - 1 Verti	0 MAIN	1 UV	01 HPF
#define	MVF_Y_LPF_SZ 		(AD == 1) ? 1056 :1088 					//01 - 1 Verti	0 MAIN	0  Y	00 LPF
#define	MVF_Y_HPF_SZ 		(AD == 1) ? 1056 :1088 					//09 - 1 Verti	0 MAIN	0  Y	01 HPF
#define	PVF_UV_LPF_SZ 		(AD == 1) ? 136  :0						//07 - 1 Verti	1  PIP	1 UV	00 LPF
#define	PVF_UV_HPF_SZ 		(AD == 1) ? 136  :0						//0f - 1 Verti	1  PIP	1 UV	01 HPF
#define	PVF_Y_LPF_SZ 		(AD == 1) ? 136  :0						//03 - 1 Verti	1  PIP	0  Y	00 LPF
#define	PVF_Y_HPF_SZ 		(AD == 1) ? 136  :0						//0b - 1 Verti	1  PIP	0  Y	01 HPF
#if (FEATURE_SHARPNESS == REV6)
#define	MHF_UV_LPF_SZ	 	(AD == 1) ? 576:0						//04 - 0 Horis	0 MAIN	1 UV	00 LPF
#define	MHF_UV_HPF_SZ	 	(AD == 1) ? 576:0						//0c - 0 Horis	0 MAIN	1 UV	01 HPF
#define	MHF_Y_LPF_SZ		(AD == 1) ? 576:0						//00 - 0 Horis	0 MAIN	0  Y	00 LPF
#define	MHF_Y_HPF_SZ 		(AD == 1) ? 576:0						//08 - 0 Horis	0 MAIN	0  Y	01 HPF
#endif
#define	PHF_UV_LPF_SZ 		(AD == 1) ? 576:0						//06 - 0 Horis	1  PIP	1 UV	00 LPF
#define	PHF_UV_HPF_SZ 		(AD == 1) ? 576:0						//0e - 0 Horis	1  PIP	1 UV	01 HPF
#define	PHF_Y_LPF_SZ 		(AD == 1) ? 576:0						//02 - 0 Horis	1  PIP	0  Y	00 LPF
#define	PHF_Y_HPF_SZ 		(AD == 1) ? 576:0						//0a - 0 Horis	1  PIP	0  Y	01 HPF


///////////////* define table acces bridge selection */////////////////
#if ((FEATURE_SHARPNESS != REV1) && (FEATURE_SHARPNESS != REV2))
#define SEL_MHF  gmd_TAB_MHF
#endif

#if ((FEATURE_SHARPNESS != REV1)&&(FEATURE_SHARPNESS != REV2))
#define SEL_MVF gmd_TAB_MVF
#endif

#if (FEATURE_SHARPNESS == REV6)
#define SEL_PVF	gmd_TAB_PVF
#define SEL_PHF	gmd_TAB_PHF
#else
#define SEL_PVF	0xff // PIP filters are not supported in FLI8532 and FLI8538
#define SEL_PHF	0xff
#endif

#if (FEATURE_SHARPNESS == REV6)
#define BOOST_MHF	MHF_SHARPNESS
#define BOOST_MVF	MVF_SHARPNESS
#define BOOST_PHF	PHF_SHARPNESS
#define BOOST_PVF	PVF_SHARPNESS
#else
#define BOOST_MHF	0
#define BOOST_MVF	0
#define BOOST_PHF	0
#define BOOST_PVF	0
#endif

//-------------------------------------------------------
// table consits the necessery data ffor sharpness filter table access:
// first column- chip mark: AD - for adavanced only; PL - for advanced and plus; CR - for all cortez chips
// second column - offset from base address of the selected table
// third column - filter data size
// forth column - filter selector for table access bridge
//------------------------------------------------------
t_FilterAccesData ROM a_FiltersTab[] =
{
   {PL,	MHF_Y_LPF_OF,	MHF_Y_LPF_SZ,	SEL_MHF, BOOST_MHF},	//00 - 0 Horis	0 MAIN	0  Y	00 LPF
   {CR,	MVF_Y_LPF_OF,	MVF_Y_LPF_SZ,	SEL_MVF, BOOST_MVF},	//01 - 1 Verti	0 MAIN	0  Y	00 LPF
   {AD,	PHF_Y_LPF_OF,	PHF_Y_LPF_SZ,	SEL_PHF, BOOST_PHF},	//02 - 0 Horis	1  PIP	0  Y	00 LPF
   {AD,	PVF_Y_LPF_OF,	PVF_Y_LPF_SZ,	SEL_PVF, BOOST_PVF},	//03 - 1 Verti	1  PIP	0  Y	00 LPF
   {PL,	MHF_UV_LPF_OF,	MHF_UV_LPF_SZ,	SEL_MHF, BOOST_MHF+1},	//04 - 0 Horis	0 MAIN	1 UV	00 LPF
   {CR,	MVF_UV_LPF_OF,	MVF_UV_LPF_SZ,	SEL_MVF, BOOST_MVF+1},	//05 - 1 Verti	0 MAIN	1 UV	00 LPF
   {AD,	PHF_UV_LPF_OF,	PHF_UV_LPF_SZ,	SEL_PHF, BOOST_PHF+1},	//06 - 0 Horis	1  PIP	1 UV	00 LPF
   {AD,	PVF_UV_LPF_OF,	PVF_UV_LPF_SZ,	SEL_PVF, BOOST_PVF+1},	//07 - 1 Verti	1  PIP	1 UV	00 LPF
   {PL,	MHF_Y_HPF_OF,	MHF_Y_HPF_SZ,	SEL_MHF, BOOST_MHF},	//08 - 0 Horis	0 MAIN	0  Y	01 HPF
   {CR,	MVF_Y_HPF_OF,	MVF_Y_HPF_SZ,	SEL_MVF, BOOST_MVF},	//09 - 1 Verti	0 MAIN	0  Y	01 HPF
   {AD,	PHF_Y_HPF_OF,	PHF_Y_HPF_SZ,	SEL_PHF, BOOST_PHF},	//0a - 0 Horis	1  PIP	0  Y	01 HPF
   {AD,	PVF_Y_HPF_OF,	PVF_Y_HPF_SZ,	SEL_PVF, BOOST_PVF},	//0b - 1 Verti	1  PIP	0  Y	01 HPF
   {PL,	MHF_UV_HPF_OF,	MHF_UV_HPF_SZ,	SEL_MHF, BOOST_MHF+1},	//0c - 0 Horis	0 MAIN	1 UV	01 HPF
   {CR,	MVF_UV_HPF_OF,	MVF_UV_HPF_SZ,	SEL_MVF, BOOST_MVF+1},	//0d - 1 Verti	0 MAIN	1 UV	01 HPF
   {AD,	PHF_UV_HPF_OF,	PHF_UV_HPF_SZ,	SEL_PHF, BOOST_PHF+1},	//0e - 0 Horis	1  PIP	1 UV	01 HPF
   {AD,	PVF_UV_HPF_OF,	PVF_UV_HPF_SZ,	SEL_PVF, BOOST_PVF+1}	//0f - 1 Verti	1  PIP	1 UV	01 HPF
};


//******************************************************************************/
//
// FUNCTION     :   BYTE Filters_MsgHandler(MsgPacket_t *message, BYTE comm_use)
// USAGE        :   This function parse the SMT messages
//
// INPUT        :   MsgPacket_t *message = SMT message
//                  BYTE comm_use  = SERIAL_USE or DDC2BI_USE
// OUTPUT       :   OK = if no error,
//
//******************************************************************************
#pragma argsused
BYTE Filters_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE far * Bp_msg = (BYTE *)message->msgpkt;
   Filters_CommandType B_Command;
   Filters_ContextType far * p_Context;

   BYTE far * len = GetOutMsgLenPtr();
   BYTE far * Bp_Reply =  GetOutMsgBufPtr();
   union
   {
      WORD  w;
      BYTE  b[2];
   } WordVal;

   // get buffer to store data beetwen different messages
   // responds 0 if new buffer is allocated,  responds 1 if buffer is already in use or 0xff if error
   BYTE B_MemAllocResult = GetBuffer((BYTE far * far *)&p_Context, sizeof(Filters_ContextType));

   //respond an error if application is not completed initialisation.
	if( /*!(gm_ReadRegWord(CLOCK_CONFIG1) & ODP_CLK_SEL_DCLK) || */
         !(gm_ReadRegWord(DISPLAY_CONTROL) & DTG_RUN_EN)    ||
         !gm_TimingIsStable(CH_A) )
   {
      //gm_Printf("Clock Err",0);
      return PORT_MSG_HNDLR_ERROR;
   }


   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context

      if (p_Context == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      //gm_Printf("Filters_MsgHandler %d",Bp_msg[0]);
      // buffer initialisation
      _fmemset (p_Context,0,sizeof(Filters_ContextType));

      // get command id from input buffer
      WordVal.b[0] = Bp_msg[0];
      WordVal.b[1] = Bp_msg[1];
      p_Context->CurrentCommand = B_Command = (Filters_CommandType)WordVal.w;
   }
   else if (B_MemAllocResult == 1 )
   {	// buffer is already in use. Data from previous message is stored in the buffer

      // respond a protocol failure if previous message was handled with an error
      if (p_Context->err)
      {
         return PORT_MSG_HNDLR_ERROR;
      }

      //restore command ID from context
      B_Command = p_Context->CurrentCommand;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((len == NULL_PTR) || (Bp_msg == NULL_PTR))
      return (BYTE)PORT_MSG_HNDLR_ERROR;

   Bp_Reply[0] = Filters_OK;
   *len = 1; //output message length

   //parse command */
   switch (B_Command)
   {
      case Filters_Write:
         {
            WORD TabOffs;
            WORD TabSize;
            WORD TabId;
            WORD Status;
            BYTE far * pTransferTable = Bp_msg;
            WORD W_Size = GetMesageLength();

            //gm_Printf("Filters_UploadFiltersTable",0);

            Bp_Reply [0] = Filters_OK;
            *len = 1;

            //handle the first message in sequence
            if (p_Context->Shift == 0)
            {
               //Handle the first message in the sequense
               Filters_WriteType * msg = (Filters_WriteType *) Bp_msg;

               //Data buffer is too small, can not be handled
               if (W_Size < sizeof(Filters_WriteType))
                  return PORT_MSG_HNDLR_ERROR;

               //store and convert filter type selectors
               p_Context->Filter.sel.Dir = msg->Dir;//Vertical or Horizontal
               p_Context->Filter.sel.Path = msg->Path;//MAIN or PIP
               p_Context->Filter.sel.Channel = msg->Channel;//Y or UV
               p_Context->Filter.sel.RespType = msg->RespType;//LPF, HPF or combined

               //The variable below is an integrated index based on selection variables above. It must be used for getting table access data from table a_FiltersTab[]
               p_Context->Filter.type = 0xf & p_Context->Filter.type;
#ifdef	DEBUG_MSG
               gm_Printf("Index = %d",p_Context->Filter.type);
               gm_Printf("AD = %d; PL = %d; CR = %d",AD,PL,CR);
               gm_Printf("ChipId = %d",a_FiltersTab[p_Context->Filter.type].chip);
               gm_Printf("TabSize = %d",a_FiltersTab[p_Context->Filter.type].size);
               gm_Printf("TabOffs = %d",a_FiltersTab[p_Context->Filter.type].offs);
               gm_Printf("TabId = %d",a_FiltersTab[p_Context->Filter.type].tab);
#endif
               //Check if the chip can support downloaded filter
               if ( (a_FiltersTab[p_Context->Filter.type].chip == 0) || (a_FiltersTab[p_Context->Filter.type].tab == 0xff))
               {
                  //gm_Printf("Filter is not supported %d; Table bridge ID = 0x%x", a_FiltersTab[p_Context->Filter.type].chip, a_FiltersTab[p_Context->Filter.type].tab);
                  Bp_Reply[0] = Filters_InvalidParameter;
                  p_Context->err = 1;
                  return PORT_MSG_HNDLR_OK;
               }

               //Write Boosting Value
               //modify buffer pointer and size to get filter table data
               pTransferTable += sizeof(Filters_WriteType)-sizeof(WORD);
               W_Size -= sizeof(Filters_WriteType)-sizeof(WORD);

               //Set boosting register
#if (FEATURE_SHARPNESS == REV6)
               if (a_FiltersTab[p_Context->Filter.type].boost > 1)
                  gm_WriteRegByte(a_FiltersTab[p_Context->Filter.type].boost, msg->Boosting);
#endif
            }


            TabSize = a_FiltersTab[p_Context->Filter.type].size;

            //cut off data if it is too long for  table
            if ((W_Size + p_Context->Shift + p_Context->StoredFlag) > TabSize)
            {
               W_Size = TabSize-p_Context->Shift;
               Bp_Reply[0] = Filters_InvalidParameter;
            }

            //copy input data bufer with taken in mind stored previous data - part of uncompleted table component (low byte of a WORD)
            if (p_Context->StoredFlag == 1)
            {
               TempBuffer[0] = p_Context->DataStored;
            }
            _fmemcpy(&TempBuffer[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);
            W_Size += p_Context->StoredFlag;

            TabId = a_FiltersTab[p_Context->Filter.type].tab;

            //Load table data.
            //if Combined filter is selcted load same data to both LPF and HPF
            //length of loaded block must be EVEN
            {
               FilterType fltr;
               fltr.type = p_Context->Filter.type;
               if ( (p_Context->Filter.sel.RespType == Filters_LPF) || (p_Context->Filter.sel.RespType == Filters_COMB) )
               {
                  fltr.sel.Channel = Filters_LPF;
                  TabOffs = a_FiltersTab[fltr.type].offs + p_Context->Shift;
                  gm_LoadSRAMBlockEx(TabId, gmd_OCM_POLLING_MODE, gmd_HIGH_PRIORITY_MODE, (WORD far*)TempBuffer, TabOffs, W_Size & 0xfffe);
               }

               // Prepare ALL ZERO buffer to clean up HPF when combination filter is selected
               if (p_Context->Filter.sel.RespType == Filters_COMB)
                  _fmemset (TempBuffer,0,W_Size);

               if ( (p_Context->Filter.sel.RespType == Filters_HPF) || (p_Context->Filter.sel.RespType == Filters_COMB) )
               {
                  fltr.sel.Channel = Filters_HPF;
                  TabOffs = a_FiltersTab[fltr.type].offs + p_Context->Shift;
                  gm_LoadSRAMBlockEx(TabId, gmd_OCM_POLLING_MODE, gmd_HIGH_PRIORITY_MODE, (WORD far*)TempBuffer, TabOffs, W_Size & 0xfffe);
               }
            }

            //store the number of loaded bytes
            p_Context->Shift += W_Size;

            //if data size is ODD store the last byte for next message to complete WORD type element
            if (W_Size%sizeof(WORD))
            {
               p_Context->StoredFlag= 1;
               p_Context->DataStored = Bp_msg[W_Size-1];
            }
            else
               p_Context->StoredFlag= 0;

            // turn on fil if it is the last message
            //if(!MoreDataExpected())
            //{
            //	gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);
            //}// */

            break;
         }// */

      case Filters_Read:
         {
            WORD  W_TransferSyze = GetMaxDataLength()&(-2);// -2 = 0xFFFE  - make the length of ouput message even
            Filters_ReadRequestType* msg = (Filters_ReadRequestType *) Bp_msg;
            Filters_ReadResponseType* out = (Filters_ReadResponseType*)Bp_Reply;
            WORD TabSize;
            WORD TabOffs;
            WORD TabID;

            if (p_Context->Shift == 0)
            {//Prepare the first message in the sequense
               // only the first element of table is transfered in the first message

               if (W_TransferSyze < sizeof(Filters_ReadRequestType))
                  return PORT_MSG_HNDLR_ERROR;

               //store and convert filter type selectors
               p_Context->Filter.sel.Dir = msg->Dir;//Vertical or Horizontal
               p_Context->Filter.sel.Path = msg->Path;//MAIN or PIP
               p_Context->Filter.sel.Channel = msg->Channel;//Y or UV
               p_Context->Filter.sel.RespType = 0x1 & msg->RespType;//LPF or HPF. Combined is ignored

               //The variable below is an integrated index based on selection variables above. It must be used for getting table access data from table a_FiltersTab[]
               p_Context->Filter.type &= 0xf;

               //gm_Printf("",0);
               //gm_Printf("AD = %d; PL = %d CR = %d",AD, PL, CR);

               //gm_Printf("",0);
               //gm_Printf("Filter Read",0);
               //gm_Printf("Index = %d",p_Context->Filter.type);
               //gm_Printf("TabSize = %d (0x%x)", a_FiltersTab[p_Context->Filter.type].size, a_FiltersTab[p_Context->Filter.type].size);
               //gm_Printf("TabOffs = 0x%x", a_FiltersTab[p_Context->Filter.type].offs);
               //gm_Printf("TabId = %d", a_FiltersTab[p_Context->Filter.type].tab);
               //gm_Printf("Chip support = %d",a_FiltersTab[p_Context->Filter.type].chip);

               //Check if the chip can support requested filter
               if ( (a_FiltersTab[p_Context->Filter.type].chip == 0) || (a_FiltersTab[p_Context->Filter.type].tab == 0xff))
               {
                  //gm_Printf("Filter is not supported %d; Table bridge ID = 0x%x", a_FiltersTab[p_Context->Filter.type].chip, a_FiltersTab[p_Context->Filter.type].tab);
                  Bp_Reply[0] = Filters_InvalidParameter;
                  p_Context->err = 1;
                  return PORT_MSG_HNDLR_OK;
               }

               // prepare response data (header)
               out->ErrID = Filters_OK;
               out->Length = a_FiltersTab[p_Context->Filter.type].size/2;

               //modify pointer to output table data buffer because some bytes are used for header
               (BYTE*)out += sizeof(Filters_ReadResponseType) - sizeof(WORD);

               // first message response length
               *len = sizeof(Filters_ReadResponseType);

               // during the first reading message only one element of table (2 bytes) will be transfered
               W_TransferSyze = sizeof(WORD);


            }
            else
            {
               TabSize = a_FiltersTab[p_Context->Filter.type].size;
               W_TransferSyze = (W_TransferSyze < TabSize -p_Context->Shift) ? W_TransferSyze : TabSize -p_Context->Shift;
               *len = (BYTE)W_TransferSyze;
               //gm_Printf("",0);
               //gm_Printf("TabSize = %d", TabSize);
            }

            TabOffs = a_FiltersTab[p_Context->Filter.type].offs + p_Context->Shift;
            TabID = a_FiltersTab[p_Context->Filter.type].tab;

            //use TempBuffer first because output buffer Bp_out is BYTE aligned
            if (OK != gm_UploadSRAMBlock(TabID,	gmd_OCM_POLLING_MODE,	gmd_HIGH_PRIORITY_MODE,
                                         (WORD far *) TempBuffer, TabOffs, W_TransferSyze))
               return PORT_MSG_HNDLR_ERROR;

            memcpy((BYTE*)out, (BYTE*)TempBuffer, W_TransferSyze);

            //store number of received words to be used in the next message
            p_Context->Shift += W_TransferSyze;

            // inform transport if not all expected data has been transmited
            MoreData(p_Context->Shift <TabSize);


            break;
         }

      default:

         // wrong command code received
         Bp_Reply[0] = Filters_InvalidParameter;
   }


   //store current command ID in the context.
//	p_Context->CurrentCommand = B_Command;

   gm_WriteRegWord(HOST_CONTROL,ODP_SYNC_UPDATE);

   return PORT_MSG_HNDLR_OK;
}

//*********************************  END  **************************************/
#endif //USE_SMT
