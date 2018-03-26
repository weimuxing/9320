#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"

#ifdef USE_SMT


//#define TS_LBC_LCT_DEBUG_MSG
#if defined(TS_LBC_LCT_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif

#define TS_LBC_LCT_VER 0
#define NUMBER_OF_ELEMENTS_IN_GAMMA_LUT 1024
#define GAMMA_TABLE_WIDTH 14

#define OUTPUT_GAMMA_R_MEM_START 0x1D800L
//#define OUTPUT_GAMMA_R_STEEP_MEM_START 0x1DC00L
#define OUTPUT_GAMMA_G_MEM_START 0x1E000L
//#define OUTPUT_GAMMA_G_STEEP_MEM_START 0x1E400L
#define OUTPUT_GAMMA_B_MEM_START 0x1E800L
//#define OUTPUT_GAMMA_B_STEEP_MEM_START 0x1EC00L

#define VERSION_0   0
#define VERSION_1   1
#define VERSION_2   2

#ifdef ST_4K2K_93xx_BOARD
    #define SMT_LBC_VERSION VERSION_2
    //#define LBC_SHOWBORDER_BY_R_L       //use Athena_L and Athena_R to do LBC_ShowBorder (image has an middle vertical line, not good)
                                        //undefine to use Athena_FE to do LBC_ShowBorder

    //SMT_LBC_SHOWBORDER_WIDTH_WA is to compensate border width
    //border is drawn by Athena_FE. if Athena_FE always outputs native timings to Athena_R / L (FE does not scale), it needs the workaround to make the
    //border width 2x of the SMT command width
    //for example, SMT command width is 6 pixels, the border width show on 4K panel should be 12 pixels
    //to achieve this, this workaround needs the following assumption
    //1. FE outputs native graphic timings (vertical height is less than 1080)
    //2. R / L output 1920x2160
    #define SMT_LBC_SHOWBORDER_WIDTH_WA
    #ifdef SMT_LBC_SHOWBORDER_WIDTH_WA
        #define ATHENA_R_L_OUTPUT_WIDTH     3840    //1920*2
        #define ATHENA_R_L_OUTPUT_HEIGHT    2160
        #define BORDER_WIDTH_SCALE_RATIO    2
    #endif
#else
    #define SMT_LBC_VERSION VERSION_0
#endif

typedef enum
{
   TS_LBC_LCT_START,
   TS_LBC_LCT_STOP,
   TS_LBC_LCT_READ,
   TS_LBC_LCT_WRITE,
   TS_LBC_LCT_EN,
   TS_LBC_LCT_DISABLE
}TsLBC_LCTCmd_t;

typedef enum
{
   ASIC_ID,
   GAMMA_TABLE_ENTRIES,
   GAMMA_TABLE_BITDEPTH,
   GAMMA_TABLE_COMPRESSION,
   PANEL_RESOLUTION_X,
   PANEL_RESOLUTION_Y,
   PANEL_COLOR_BITDEPTH,
   ZONES_X,
   ZONES_Y,
   RGB_LED_BACKLIGHT,
   MAX_GRID_DOTS_X,
   MAX_GRID_DOTS_Y
}LBC_Capabilities;

typedef enum 
{
    SINGLE_ATHENA = 0,
    LEFT_PANE_ATHENA = 1,
    RIGHT_PANE_ATHENA = 2
}   AthenaTargetID_t;

#pragma pack(push)
#pragma pack(1)
typedef struct SMT_Display_Grid
{
   BYTE Command;
   BYTE Command_hi;
   BYTE Color[8]; // Color in the RGB64 format
   WORD nVertLines; // Number of grid dots in X direction (0¡K65535)
   WORD nHorzLines; // Number of grid dots in Y direction (0¡K65535)
   SWORD nXOffsetLeft; // Signed short integer
   WORD nXSize; // Fractional 7.8
   SWORD nYOffsetTop; // Signed short integer
   WORD nYSize; // Fractional 7.8
}gmt_SMT_Display_Grid_Req;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct SMT_Display_Field
{
   BYTE Command;
   BYTE Command_hi;
   BYTE Color[8]; // Color in the RGB64 format
   BYTE Reserved[12];
}gmt_SMT_Display_Field_Req;
#pragma pack(pop)

#pragma pack(push)
#pragma pack(1)
typedef struct WriteGammaTable_Req
{
   BYTE Command;
   DWORD TotalSize;
   BYTE VersionNumber;
   BYTE CompEnable; // for diamond is always 1
   WORD LengthOfEachColor; // length of main LUT in 16 bit words. - for diamond is 512 max
   BYTE OffSetOrPtr; // 0 - unions in this structure contain offsets
   DWORD GammaTableoffsetGreen; // Offset to the GREEN Gamma table from the beginning of this table
   DWORD GammaTableoffsetBlue; // Offset to the BLUE Gamma table from the beginning of this table
   DWORD GammaTableoffsetRed; // Offset to the RED Gamma table from the beginning of this table
   BYTE Reserved[3];
   WORD SteepTableSize; // Steep table size in bytes for each color. for  diamond the size is 512 bytes max
   WORD SteepStep; // 16 bit enum
   WORD GreenSteepStart;
   WORD GreenSteepEnd;
   WORD GreenSteepFinal;
   WORD BlueSteepStart;
   WORD BlueSteepEnd;
   WORD BlueSteepFinal;
   WORD RedSteepStart;
   WORD RedSteepEnd;
   WORD RedSteepFinal;
   DWORD SteepTableoffsetGreen; // Offset to the GREEN steep table
   DWORD SteepTableoffsetBlue; // Offset to the BLUE steep table
   DWORD SteepTableoffsetRed; // Offset to the RED steep table
}gmt_WriteGammaTable_Req;
#pragma pack(pop)

extern gmt_RET_STAT gm_LoadDUMTable(WORD W_StartIndex, WORD W_Size, WORD far *Wp_Data);

static PortMsgHndlrReplyType LBC_SMT_ShowBorder(BYTE far * Bp_SMTMsg);

#ifdef ST_4K2K_93xx_BOARD
    #ifdef LBC_SHOWBORDER_BY_R_L
        static WORD DH_Active_Start_Saved = 0;
    #endif
void SMT_SendTimeoutTimetoHost(MsgPacket_t *message, BYTE comm_use, WORD Timeout);
#endif

#pragma argsused
BYTE LBC_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE far * Bp_msg = (BYTE *)message->msgpkt;
   BYTE far * Bp_Len = GetOutMsgLenPtr();
   BYTE far * Bp_Reply =  GetOutMsgBufPtr();
   LBC_LCTContextType far * p_Context;
   LBC_CommandType B_Command;
	PortMsgHndlrReplyType ErrorCode = PORT_MSG_HNDLR_OK;

   // get buffer to store data beetwen different messages
   // responds 0 if new buffer is allocated,  responds 1 if buffer is already in use or 0xff if error
   BYTE B_MemAllocResult = GetBuffer((BYTE far * far *)&p_Context, sizeof(LBC_LCTContextType));

   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context
      if (p_Context == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      // buffer initialisation
		_fmemset (p_Context,0,sizeof(LBC_LCTContextType));
      p_Context->FirstMessage = 1;

      // get command id from input buffer
      B_Command = (LBC_CommandType)Bp_msg[0];

      p_Context->CurrentCommand = B_Command;
   }
   else if (B_MemAllocResult == 1 )
   {// buffer is already in use. Data from previous message is stored in the buffer
      //use command ID from context if is not the first message in multy message transport transaction
      dbgmsg(" More ",0);
      B_Command = (LBC_CommandType)p_Context->CurrentCommand;
      p_Context->FirstMessage = 0;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      dbgmsg("LBC: ERROR memalloc 1",0);
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((Bp_Len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      dbgmsg("LBC: ERROR memalloc 2",0);
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_Reply[0] = LBC_LCT_OK;
   *Bp_Len = 1; //output message length

   //parse command
   switch (B_Command)
   {
      case LBC_Start:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
       		//inter-chip communication to Athena_L/FE
				SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
				if(ErrorCode == PORT_MSG_HNDLR_OK)
        	   	ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS);

           	#ifdef LBC_SHOWBORDER_BY_R_L
   			DH_Active_Start_Saved = gm_ReadRegWord(DH_ACTIVE_START);
   			#endif
			}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
			#endif // 
         {
            // set DebugOn mode
            gmvb_DebugOnly = TRUE;            
            
            // Bp_msg[0] // command

            // version
				dbgmsg("version 0x%x", Bp_msg[1]);

            // size
				dbgmsg("size 1  0x%x", Bp_msg[2]);
            dbgmsg("size 2 0x%x", Bp_msg[3]);

            // LEDCDB??

            // Bypass ODP 3x3 and disable DUM
            gm_ClearRegBitsWord(GAMMA_LUT2_CTRL, GAMMA_MATRIX_CTRL);
            gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
            //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
            gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
            

            // version
            Bp_Reply[1] = SMT_LBC_VERSION;

    			#ifdef ST_4K2K_93xx_BOARD
            Bp_Reply[0] = LBC_LCT_OK;
            Bp_Reply[2] = 1;//two Athena
            *Bp_Len = 3; //output message length
    			#else
            *Bp_Len = 2; //output message length
    			#endif
         }
         break;

      case LBC_End:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
       		//inter-chip communication to Athena_L/FE
				SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
				if(ErrorCode == PORT_MSG_HNDLR_OK)
        	   	ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS);
           	#ifdef LBC_SHOWBORDER_BY_R_L
   			gm_WriteRegWord(DH_ACTIVE_START, DH_Active_Start_Saved);
   			#endif
			}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
			#endif // 
         {
            // Enable ODP Color Process for all Pixels
            //gm_WriteRegWord(DISP_LUT_CONTROL, BIT0|BIT1);
   			//gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
   			gm_WriteRegWord(IMP_PATGEN_CONTROL, 0x0); // Disable IMP Pattern
   			gm_ClearRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);  //disable highlighted border
            //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
            gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate

            // no data to responce
            *Bp_Len = 0;
            dbgmsg("LBC_End",0)    ;
            
            // set DebugOff mode
            gmvb_DebugOnly = FALSE;            
         }
         break;

      case LBC_GetCapabilities:
         {
            union
            {
               DWORD  dw;
               BYTE  b[4];
            }DWordVal;

            *Bp_Len = 5;

            // Bp_msg[0] // command
				// Bp_msg[1] // command
				// Bp_msg[2~5] // capabilities ID
				//dbgmsg(" 0 = 0x%x", Bp_msg[0]);
				//dbgmsg(" 1 = 0x%x", Bp_msg[1]);
				//dbgmsg(" 2 = 0x%x", Bp_msg[2]);
				//dbgmsg(" 3 = 0x%x", Bp_msg[3]);
				//dbgmsg(" 4 = 0x%x", Bp_msg[4]);
            //dbgmsg(" 5 = 0x%x", Bp_msg[5]);

				switch(Bp_msg[2])
            {
               case ASIC_ID: // ??
                  Bp_Reply[1] = 0;
                  Bp_Reply[2] = 0;
                  Bp_Reply[3] = 0;
                  Bp_Reply[4] = 0;
                  break;

               case GAMMA_TABLE_ENTRIES:
                  DWordVal.dw = 1024;
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case GAMMA_TABLE_BITDEPTH:
                  DWordVal.dw = 14;
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case GAMMA_TABLE_COMPRESSION: // ??
                  Bp_Reply[1] = 0;
                  Bp_Reply[2] = 0;
                  Bp_Reply[3] = 0;
                  Bp_Reply[4] = 1; // 0 - Uncompressed data, Other - Type of Gamma Table compression
                  break;

               case PANEL_RESOLUTION_X:
                  DWordVal.dw = PanelWidth;
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case PANEL_RESOLUTION_Y:
                  DWordVal.dw = PanelHeight;
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case PANEL_COLOR_BITDEPTH:
                  DWordVal.dw = PanelDepth;
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case ZONES_X:
                  DWordVal.dw = 1; // ??
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case ZONES_Y:
                  DWordVal.dw = 1; // ??
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case RGB_LED_BACKLIGHT: // ??
                  // 0- Only B/W LED/LCD backlight is supported
                  // 1 - RGB backlight is supported, i.e. RGB are individually controllable
                  Bp_Reply[1] = 0;
                  Bp_Reply[2] = 0;
                  Bp_Reply[3] = 0;
                  Bp_Reply[4] = 0;
                  break;

               case MAX_GRID_DOTS_X:
                  DWordVal.dw = 20; // C9VODP_DUM_NUM_GRIDS
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               case MAX_GRID_DOTS_Y:
                  DWordVal.dw = 20; // C9VODP_DUM_NUM_GRIDS
                  Bp_Reply[1] = DWordVal.b[0];
                  Bp_Reply[2] = DWordVal.b[1];
                  Bp_Reply[3] = DWordVal.b[2];
                  Bp_Reply[4] = DWordVal.b[3];
                  break;

               default:
                  // wrong command code received
                  Bp_Reply[0] = LBC_LCT_NotSupported;
                  *Bp_Len = 1;
            }
         }
         break;

      case LBC_DisplayGrid:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
       		//inter-chip communication to Athena_FE
//          ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
            ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS);
        	}
			
			if(ErrorCode == PORT_MSG_HNDLR_OK)
            if(GetChipID_4K2K()==CHIPID_4K2K_FE)
    		#endif
         {
            gmt_SMT_Display_Grid_Req * pReq = (gmt_SMT_Display_Grid_Req *)Bp_msg;
            union
            {
               WORD  w;
               BYTE  b[2];
            }WordVal;

            #if defined(TS_LBC_LCT_DEBUG_MSG)
            BYTE i = 0;

            for (; i < 22 ; i++)
            {
					gm_Printf("%d = 0x%x", i, Bp_msg[i]);
            }
            #endif

            // Enable DUM
            gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE|DUM_GRID_DOTS);

            // R channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[0];
            WordVal.b[1] = pReq->Color[1];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_RED, WordVal.w&DUM_GLOBAL_RED);

            // G channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[2];
            WordVal.b[1] = pReq->Color[3];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_GRN, (((DWORD)WordVal.w << 13)&DUM_GLOBAL_GRN) );

            // B channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[4];
            WordVal.b[1] = pReq->Color[5];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);

            // White LED brightness, unsigned short value // ??
            WordVal.b[0] = pReq->Color[6];
            WordVal.b[1] = pReq->Color[7];

            // gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);

            gm_ClearAndSetBitsDWord(C9VODP_DUM_NUM_GRIDS, DUM_NUM_XGRIDS, pReq->nVertLines&DUM_NUM_XGRIDS);
            gm_ClearAndSetBitsDWord(C9VODP_DUM_NUM_GRIDS, DUM_NUM_YGRIDS, ((((DWORD)pReq->nHorzLines) << 9)&DUM_NUM_YGRIDS));

            pReq->nXSize = PanelWidth / pReq->nVertLines;
				dbgmsg("pReq->nXSize = 0x%x", pReq->nXSize);
            pReq->nYSize = PanelHeight / pReq->nHorzLines;
				dbgmsg("pReq->nYSize = 0x%x", pReq->nYSize);

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GRID_SZ, DUM_XGRID_SIZE, pReq->nXSize&DUM_XGRID_SIZE);
            gm_ClearAndSetBitsDWord(C9VODP_DUM_GRID_SZ, DUM_YGRID_SIZE, ((DWORD)pReq->nYSize&DUM_XGRID_SIZE) << 12);

            gm_ClearAndSetBitsDWord(C9VODP_DUM_RSTART, DUM_RSTART_XGRID, pReq->nXOffsetLeft&DUM_RSTART_XGRID); // ??
            // pReq->nYOffsetYTop // ??

            // what condition to reply error // ??
         }
         break;

      case LBC_DisplayField:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
//          ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
            ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
            if(GetChipID_4K2K()==CHIPID_4K2K_FE)
    		#endif
         {
            gmt_SMT_Display_Field_Req * pReq = (gmt_SMT_Display_Field_Req *)Bp_msg;
            union
            {
               WORD  w;
               BYTE  b[2];
            }WordVal;

            dbgmsg("LBC Display Field", 0);

            // Enable DUM
            //gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE|DUM_3_COLOR);

            // Diable ODP Color Process
            //gm_ClearRegBitsWord(DISP_LUT_CONTROL, 0);            
            #if 0 // Enable/Disable via new command
            gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
            #endif
            //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);            
            gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate

            gm_WriteRegWord(IMP_PATGEN_CONTROL, 0xE0); // Enable IMP Pattern Gen as background color

            // R channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[0];
            WordVal.b[1] = pReq->Color[1];

            dbgmsg(" R 0x%x", WordVal.w);

            //gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_RED, WordVal.w&DUM_GLOBAL_RED);
            gm_WriteRegWord(IMP_PATGEN_RED, WordVal.b[1]); // R

            // G channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[2];
            WordVal.b[1] = pReq->Color[3];

            dbgmsg(" G 0x%x", WordVal.w);

            //gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_GRN, (WordVal.w&DUM_GLOBAL_RED) << 13);
            gm_WriteRegWord(IMP_PATGEN_GRN, WordVal.b[1]); // G

            // B channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[4];
            WordVal.b[1] = pReq->Color[5];

            dbgmsg(" B 0x%x", WordVal.w);

            //gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);
            gm_WriteRegWord(IMP_PATGEN_BLUE, WordVal.b[1]); // B

            // White LED brightness, unsigned short value // ??
            WordVal.b[0] = pReq->Color[6];
            WordVal.b[1] = pReq->Color[7];

            // gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);

            // what condition to reply error // ??
         }
         break;

      case LBC_DisplayZone:
         {
            //gm_Printf("~~~~ Not Support in Athena ...", 0);
         }
         break;

      case LBC_SetBKColor:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
       		//inter-chip communication to Athena_L
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            gmt_SMT_Display_Field_Req * pReq = (gmt_SMT_Display_Field_Req *)Bp_msg;
            union
            {
               WORD  w;
               BYTE  b[2];
            }WordVal;

#if 1
            gm_WriteRegWord(IMP_PATGEN_CONTROL, 0xE0); // Enable IMP Pattern Gen as background color
            WordVal.b[0] = pReq->Color[0];
            WordVal.b[1] = pReq->Color[1];

            gm_WriteRegWord(IMP_PATGEN_RED, WordVal.w); // R

            WordVal.b[0] = pReq->Color[2];
            WordVal.b[1] = pReq->Color[3];

            gm_WriteRegWord(IMP_PATGEN_GRN, WordVal.w); // G

            WordVal.b[0] = pReq->Color[4];
            WordVal.b[1] = pReq->Color[5];

            gm_WriteRegWord(IMP_PATGEN_BLUE, WordVal.w); // B

            WordVal.b[0] = pReq->Color[6];
            WordVal.b[1] = pReq->Color[7];

            // White??

#else
            // Enable DUM
            gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE|DUM_3_COLOR);

            // how to support this // ??

            // R channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[0];
            WordVal.b[1] = pReq->Color[1];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_RED, WordVal.w&DUM_GLOBAL_RED);

            // G channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[2];
            WordVal.b[1] = pReq->Color[3];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL0, DUM_GLOBAL_GRN, (WordVal.w&DUM_GLOBAL_RED) << 13);

            // B channel brightness, unsigned short value
            WordVal.b[0] = pReq->Color[4];
            WordVal.b[1] = pReq->Color[5];

            gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);

            // White LED brightness, unsigned short value // ??
            WordVal.b[0] = pReq->Color[6];
            WordVal.b[1] = pReq->Color[7];

            // gm_ClearAndSetBitsDWord(C9VODP_DUM_GLOBAL1, DUM_GLOBAL_BLU, WordVal.w&DUM_GLOBAL_BLU);
#endif

            // what condition to reply error // ??
         }
         break;

      case LBC_GetFirmwareRevisionString:
         {
            // Size // String length (including zero) in bytes
            Bp_Reply[1] = 0;
            Bp_Reply[2] = 0;

            // String // Firmware Revision String zero-terminated)
            // what kind of string will be replied // ??

            *Bp_Len = 3; // depend on String
         }
         break;

      case LBC_ShowBorder:
         {
				union
				{
				   WORD  w;
				   BYTE  b[2];
				}WordVal;
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
    		{
            #ifdef LBC_SHOWBORDER_BY_R_L
				   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
					if(ErrorCode == PORT_MSG_HNDLR_OK)
            LBC_SMT_ShowBorder(Bp_msg);
            #else
				   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEFE_DST_ADDRESS);
            #endif
         }
			else
         {
        		#ifdef LBC_SHOWBORDER_BY_R_L            
				if(GetChipID_4K2K()==CHIPID_4K2K_L)
            LBC_SMT_ShowBorder(Bp_msg);
    		#else
         LBC_SMT_ShowBorder(Bp_msg);
    		#endif
			}
			LBC_SMT_ShowBorder(Bp_msg);
			#endif
            dbgmsg("LBC Show Border", 0);

				WordVal.b[0] = Bp_msg[2];
				WordVal.b[1] = Bp_msg[3];

            gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_RED, WordVal.w & PB_HLBRD_COLOR_RED); // R

				WordVal.b[0] = Bp_msg[4];
				WordVal.b[1] = Bp_msg[5];

            gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_GRN, (WordVal.w >> 5) & PB_HLBRD_COLOR_GRN); // G

				WordVal.b[0] = Bp_msg[6];
				WordVal.b[1] = Bp_msg[7];

            gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_BLU, (WordVal.w >> 11) & PB_HLBRD_COLOR_BLU); // B            

				//WordVal.b[0] = Bp_msg[8];
				//WordVal.b[1] = Bp_msg[9];
            // White ?

            // Width
            gm_WriteRegWord(PB_HLBRDR_WIDTH, Bp_msg[10]);
            gm_WriteRegWord(PB_HLBRDR_HACTIVE, PanelWidth - (gm_ReadRegWord(PB_HLBRDR_WIDTH)*2));

            // Height
            gm_WriteRegWord(PB_HLBRDR_HEIGHT, Bp_msg[11]);
            gm_WriteRegWord(PB_HLBRDR_VACTIVE, PanelHeight - (gm_ReadRegWord(PB_HLBRDR_HEIGHT)*2));

            #if 1
            if(Bp_msg[12])
               gm_SetRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);
            else
               gm_ClearRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);            
            #else
            if(gm_ReadRegWord(PB_HLBRDR_WIDTH) && gm_ReadRegWord(PB_HLBRDR_HEIGHT))
               gm_SetRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);
            else
               gm_ClearRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);
            #endif
            

            //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
            gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate

      	}
         break;

      case LBC_SetUnicompMode:
         {
            // Enable
            if(Bp_msg[2] == 1)
				{
               gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
               dbgmsg("!!!ENABLE!!!", 0);
				}
            else if(Bp_msg[2] == 0)
				{
               gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
               dbgmsg("!!!DISABLE!!!", 0);
				}
            else
				{
               Bp_Reply[0] = LBC_LCT_NotSupported;
               dbgmsg("!!!NOT!!!", 0);
				}
         }
    		
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
         {
       		//inter-chip communication to Athena_L
        	   ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
        	}
    		#endif
         break;        

      default:
         // wrong command code received
         Bp_Reply[0] = LBC_LCT_NotSupported;
         break;
   }

   //store current command ID in the context.
   p_Context->CurrentCommand = B_Command;

   return ErrorCode;

}

#pragma argsused // remove unused parameter warning
BYTE ACT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE far * Bp_msg = (BYTE *)message->msgpkt;
   BYTE far * Bp_Len = GetOutMsgLenPtr();
   BYTE far * Bp_Reply =  GetOutMsgBufPtr();
   LBC_LCTContextType far * p_Context;
   ACT_CommandType B_Command;
	PortMsgHndlrReplyType ErrorCode = PORT_MSG_HNDLR_OK;

   // get buffer to store data beetwen different messages
   // responds 0 if new buffer is allocated,  responds 1 if buffer is already in use or 0xff if error
   BYTE B_MemAllocResult = GetBuffer((BYTE far * far *)&p_Context, sizeof(LBC_LCTContextType));


   //gm_Printf("~ACT Handler~", 0);

   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context
      if (p_Context == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      // buffer initialisation
   	_fmemset (p_Context,0,sizeof(LBC_LCTContextType));
      p_Context->FirstMessage = 1;

      // get command id from input buffer
      B_Command = (ACT_CommandType)Bp_msg[0];

      p_Context->CurrentCommand = B_Command;
   }
   else if (B_MemAllocResult == 1 )
   {// buffer is already in use. Data from previous message is stored in the buffer
      //use command ID from context if is not the first message in multy message transport transaction
      dbgmsg(" More ",0);
      B_Command = (ACT_CommandType)p_Context->CurrentCommand;
      p_Context->FirstMessage = 0;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      dbgmsg("ACT: ERROR memalloc 1",0);
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((Bp_Len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      dbgmsg("ACT: ERROR memalloc 2",0);
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_Reply[0] = LBC_LCT_OK;
   *Bp_Len = 1; //output message length

   //dbgmsg("~ ACT B_Command = 0x%x", B_Command);

   //parse command
   switch (B_Command)
   {
      case ACT_Start:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
        		ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
			{
         // set DebugOn mode
         gmvb_DebugOnly = TRUE;
         
         // Version // Protocol version supported by the tool // which version should Athena support // ??
			dbgmsg("ACT S", 0);
			dbgmsg("version 0x%x", Bp_msg[1]);

         // Version // Highest protocol version supported by the firmware // ??
         Bp_Reply[1] = 0;
         *Bp_Len = 2;
			}
         break;

      case ACT_End:
         #ifdef ST_4K2K_93xx_BOARD
         if(GetChipID_4K2K()==CHIPID_4K2K_R)
        		ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);

			if(ErrorCode == PORT_MSG_HNDLR_OK)
    		#endif
         {
            dbgmsg("ACT End", 0);
            
            // set DebugOff mode
            gmvb_DebugOnly = FALSE;            
         }
         break;

      case ACT_WriteUniformityCompensationTable:
         {
         #ifdef ST_4K2K_93xx_BOARD
            if(GetChipID_4K2K()==CHIPID_4K2K_R)
            {
               //message->msgType :    Bit 0 -7
               //message->control  :   Bit 8 - 15
               //refer to doc "Streaming Message Transport.doc"
               //Bit               Description
               //Bit 0         1 - Control Message (CM)
               //              0 - Transport Message (TM)
               //Bit 1         Even/Odd Message Count
               //Bit 2         CM: Reserved
               //              TM: 1 - More Data Pending; 0 - No More Data
               //Bits 3-7      Reserved
               //Bits 8-15     CM: Control Message Code
               //              TM: Transaction ID

               static AthenaTargetID_t TargetID = 0;
               static BOOL IsTableDataOnlyTM = FALSE;//each TM max length is 255 bytes, the table could be split into multiple TM. the first TM contains table information
               //but the rest of TMs only contain table data, no table information
               //refer to doc "Athena Uniformity Compensation SMT Interface.doc"
                                            
               //only the first TM contains targetID
               if (IsTableDataOnlyTM == FALSE)
               {
                  //Bp_msg[0] - command low byte
                  //Bp_msg[1] - command hi byte
                  //Bp_msg[2] - version number
                  //Bp_msg[3] - ColorFormat
                  //Bp_msg[4 - 5] - TableSizeX
                  //Bp_msg[6 - 7] - TableSizeY
                  //Bp_msg[8] - targetID

                  TargetID = (AthenaTargetID_t)Bp_msg[8];//[8] - targetID Target Athena index. 
                  // 0 - single (the only) Athena
                  // 1 - left pane Athena
                  // 2 - right pane Athena
                  IsTableDataOnlyTM = TRUE;

//                 if(TargetID==LEFT_PANE_ATHENA)
//                     gm_Print("ACT_WriteUniformityCompensationTable Left Pane", 0);
//                 else if(TargetID==RIGHT_PANE_ATHENA)
//                     gm_Print("ACT_WriteUniformityCompensationTable Right Pane", 0);
//                 else
//                     gm_Print("ACT_WriteUniformityCompensationTable Single", 0);

               }
               
               if (((message->msgType) & MORE_DATA_BIT) == 0) //Bit2 : TM: 1 - More Data Pending; 0 - No More Data
                  IsTableDataOnlyTM = FALSE;  //reset the flag if it is the last TM

               if (TargetID == LEFT_PANE_ATHENA)
               {
                  SMT_SendTimeoutTimetoHost(message, comm_use, SMT_TRANSACTION_TIMEOUT);
                ErrorCode = SMT_ForwardMessageToSlave(message, SOCIPC_DEVICEL_DST_ADDRESS);
                  Bp_Reply[0] = LBC_LCT_OK;
                  *Bp_Len = 1; //output message length
                  break;  //Athena_R breaks the case if the table is for Athena_L
               }
            }
          #endif //ST_4K2K_93xx_BOARD
            {
               WORD W_Size = GetMesageLength();
               BYTE far * pTransferTable= Bp_msg;
               BYTE * Bp_TempTable = TempBuffer;

               dbgmsg("ACT W T", 0);

               if (p_Context->FirstMessage)
               {
                  WORD DUM_XGRID_SIZE1, DUM_YGRID_SIZE1, DUM_NUM_XGRIDS1, DUM_NUM_YGRIDS1, DUM_RSTART_XGRID1;
                  DWORD DUM_XGRID_IINC1, DUM_YGRID_IINC1;

                  union
                  {
                  WORD  w;
                  BYTE  b[2];
                  }WordVal;

                  // Set Grid
                  // X
                  WordVal.b[0] = Bp_msg[4];
                  WordVal.b[1] = Bp_msg[5];

                  DUM_NUM_XGRIDS1 = WordVal.w;

                  // Y
                  WordVal.b[0] = Bp_msg[6];
                  WordVal.b[1] = Bp_msg[7];

                  DUM_NUM_YGRIDS1 = WordVal.w;

                  DUM_XGRID_SIZE1 = PanelWidth / (DUM_NUM_XGRIDS1 - 1);
                  DUM_YGRID_SIZE1 = PanelHeight / (DUM_NUM_YGRIDS1 - 1);		

                  DUM_XGRID_IINC1 = (DWORD)(524288 / DUM_XGRID_SIZE1);
                  DUM_YGRID_IINC1 = (DWORD)(524288 / DUM_YGRID_SIZE1);

                  DUM_RSTART_XGRID1 = (DUM_NUM_XGRIDS1 / 2);

                  if (Bp_msg[3])
                     gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_3_COLOR);
                  else
                     gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_3_COLOR);

                  // Also can enable (DUM_GRID_DOTS | DUM_3_COLOR for test Dots and color)
                  gm_WriteRegDWord_LE(C9VODP_DUM_NUM_GRIDS, ((DWORD)DUM_NUM_YGRIDS1 << 9)|DUM_NUM_XGRIDS1);
                  gm_WriteRegDWord_LE(C9VODP_DUM_GRID_SZ, ((DWORD)DUM_YGRID_SIZE1 << 12)|(DUM_XGRID_SIZE1));
                  gm_WriteRegWord(C9VODP_DUM_GLOBAL0, 0xFFFF);
                  gm_WriteRegWord(C9VODP_DUM_GLOBAL0 + 2, 0x03FF);
                  gm_WriteRegWord(C9VODP_DUM_GLOBAL1, 0x1FFF);
                  gm_WriteRegDWord_LE(C9VODP_DUM_XGRID_IINC, DUM_XGRID_IINC1);
                  gm_WriteRegDWord_LE(C9VODP_DUM_YGRID_IINC, DUM_YGRID_IINC1);
                  gm_WriteRegWord(C9VODP_DUM_RSTART, DUM_RSTART_XGRID1);

                  //dbgmsg("version 0x%x", Bp_msg[2]);
                  //dbgmsg("ColorFormat 0x%x", Bp_msg[3]);
                  //dbgmsg("TableSizeX_lo 0x%x", Bp_msg[4]);
                  //dbgmsg("TableSizeX_Hi 0x%x", Bp_msg[5]);
                  //dbgmsg("TableSizeY_lo 0x%x", Bp_msg[6]);
                  //dbgmsg("TableSizeY_Hi 0x%x", Bp_msg[7]);               

                  // 8 & 9 are reserved

                  W_Size -= 10;
                  pTransferTable += 10;

                  // following are the data to set to memory ... RGB in 16-bit but only 13-bit are used...
                  // Use shift to save the data index ...
                  // The address should from 0x2E000
               #ifdef SAVING_DATA_INTO_FLASH
                  {
                  WORD W_RegAddr;
                  DWORD DW_FlashAddr = 0x5FC000; // 0xBFC000;


                  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
                  // Clear DUM ram
                  _fmemset(LINEAR_TO_FPTR(0x2E000L),0xFF,0x2000);   

                  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
                  UserPrefDUMSwitch = TRUE; // Set variable to enable ...

                  // Clear old data
                  //asm{ cli }
                  Flash_EraseSector(DW_FlashAddr, 3, SECT_ER_DEST_INCLUDED); // Register value saving
                  //Flash_EraseSector(0x5FD000, 1, SECT_ER_DEST_INCLUDED); // 0x2E000 data saving
                  //Flash_EraseSector(0x5FE000, 1, SECT_ER_DEST_INCLUDED); // 0x2F000 data saving

                  // Save new data            		
                  for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
                  {
                     FlashWriteWord(DW_FlashAddr, gm_ReadRegWord(W_RegAddr));
                     W_RegAddr += 2;
                     DW_FlashAddr += 2;
                  }
                  //asm{ sti }                  
                  }
               #endif
			   
			   #ifdef SAVING_DATA_INTO_NVRAM			   
               {
                  WORD W_RegAddr;
                  WORD DW_NvramAddr = UniformityRegisterSettingStart; // 0xBFC000;


                  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
                  // Clear DUM ram
                  _fmemset(LINEAR_TO_FPTR(0x2E000L),0xFF,0x2000);   

                  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
                  UserPrefDUMSwitch = TRUE; // Set variable to enable ...
                  
                  // Clear old data
                  //asm{ cli }
 //                 NVRam_WriteByte(UniformityIndexEvenStart, 0xff); // Register value saving
				{
									 
					BYTE ExpectedEvenOdd;
					BYTE B_TransactionID;
					SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;
				  
					B_TransactionID = SMT_GetTransID(message);
					pCurrentTransaction = IsTransIDValid(B_TransactionID);
												
					//gm_Printf("BUSY", 0);
					ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;
					SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,1500);
				}
				Bp_Reply[0] = LBC_LCT_OK;
				*Bp_Len = 1; //output message length
				
				  NVRam_WriteBuffer_Clear_NVRAM(0xA000,0x2100);
                  
                  // Save new data            		
                  for(W_RegAddr = C9VODP_DUM_CONTROL_0; W_RegAddr < C9VODP_DUM_RSTART; )
                  {
                     NVRam_WriteWORD(DW_NvramAddr, gm_ReadRegWord(W_RegAddr));
                     W_RegAddr += 2;
                     DW_NvramAddr += 2;
                  }
                  //asm{ sti }                  
               }
				#endif
            }

               // copy input data bufer with taken in mind stored previous data - part of uncompleted DUM table (low byte of a WORD)
               if (p_Context->StoredFlag)
               {
                  Bp_TempTable[0] = p_Context->DataStored;
               }

               //take data from input buffer
               memcpy(&Bp_TempTable[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);

               //modify Size value if data byte was stored
               W_Size += p_Context->StoredFlag;

               if (W_Size)
               {
                  gm_ClearRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE); // Must clear DUM_ENABLE for upload data.
                  gm_LoadDUMTable(p_Context->Shift, W_Size/2, (WORD far*)Bp_TempTable);
               }

               p_Context->Shift += W_Size;

               //store the last byte for next message if data size is odd
               if (W_Size%sizeof(WORD))
               {
                  p_Context->StoredFlag= 1;
                  p_Context->DataStored = Bp_TempTable[W_Size-1];
               //dbgmsg("TEST!!!!", 0);
               }
               else
               {
                  p_Context->StoredFlag= 0;
               }

            if (!MoreDataExpected())
            {
               dbgmsg("Done", 0);
               
			   #ifdef SAVING_DATA_INTO_NVRAM

#if 0
                if(B_ReturnBusy)
            	{
               		dbgmsg("BUSY", 0);
               		return PORT_MSG_HNDLR_BUSY;
            	}
#endif
 //          		if(B_ReturnBusy == 0)
            	{
               		//extern BYTE SMT_GetTransID(MsgPacket_t * message);
               		//extern SMT_TransactionStruct_t SMT_RAM * IsTransIDValid(BYTE TransID);
               		//extern void SMT_SendBusy(BYTE TransID, BYTE comm_use, BYTE EvenOdd, WORD W_Delay_ms);
               
               		BYTE ExpectedEvenOdd;
               		BYTE B_TransactionID;
               		SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;

               		B_TransactionID = SMT_GetTransID(message);
               		pCurrentTransaction = IsTransIDValid(B_TransactionID);
                              
               		//gm_Printf("BUSY", 0);
               		{
                  		ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

#if 0
                  		if(B_ODD_EVEN_OverRide)
                     	if(ExpectedEvenOdd)
                        	ExpectedEvenOdd = 0;
                     	else
                        	ExpectedEvenOdd = EVEN_ODD_BIT;
#endif
               		}
               		SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,2000);

               		//B_ReturnBusy = 1;               
            	}
				Bp_Reply[0] = LBC_LCT_OK;
   				*Bp_Len = 1; //output message length
   				
			   DUM_SAVEtoNVRAM();
			   
			   #endif
			   
               //gm_ClearRegBitsWord(DISP_LUT_CONTROL, 3);
                  gm_SetRegBitsWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
                  //gm_ClearRegBitsWord(DISP_LUT_CONTROL, 3);
                  //gm_SyncUpdate(gmvb_MainChannel, OUTPUT_CLOCK);
                  gm_SetRegBitsWord(HOST_CONTROL, ODP_SYNC_UPDATE); // debugon can't use gm_SyncUpdate
               }
               else
               {
                  dbgmsg("More", 0);
               }
            }
            #if 0 // 1
            // Set Busy delay xxxx ms ...
            //Bp_Reply[0] = 0x11; // This is transaction ID
            Bp_Reply[1] = 0x80;
            Bp_Reply[2] = 0x00;
            return PORT_MSG_HNDLR_BUSY;

            // Return BUSY to Tool then the Tool will re-send the command again.
            #endif
         }
         break;

      default:
         // wrong command code received
         Bp_Reply[0] = LBC_LCT_NotSupported;
   }

   //store current command ID in the context.
   p_Context->CurrentCommand = B_Command;

   return ErrorCode;
}

#pragma argsused // remove unused parameter warning
BYTE LCT_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
   BYTE far * Bp_msg = (BYTE *)message->msgpkt;
   BYTE far * Bp_Len = GetOutMsgLenPtr();
   BYTE far * Bp_Reply =  GetOutMsgBufPtr();
   LBC_LCTContextType far * p_Context;
   LCT_CommandType B_Command;

   // get buffer to store data beetwen different messages
   // responds 0 if new buffer is allocated,  responds 1 if buffer is already in use or 0xff if error
   BYTE B_MemAllocResult = GetBuffer((BYTE far * far *)&p_Context, sizeof(LBC_LCTContextType));

   if (B_MemAllocResult == 0 )
   {// buffer is just allocated; initialize the context
      if (p_Context == NULL_PTR)
         return PORT_MSG_HNDLR_ERROR;

      // buffer initialisation
		_fmemset (p_Context,0,sizeof(LBC_LCTContextType));
      p_Context->FirstMessage = 1;

      // get command id from input buffer
      B_Command = (LCT_CommandType)Bp_msg[0];

      p_Context->CurrentCommand = B_Command;
   }
   else if (B_MemAllocResult == 1 )
   {// buffer is already in use. Data from previous message is stored in the buffer
      //use command ID from context if is not the first message in multy message transport transaction
      dbgmsg(" More ",0);
      B_Command = (LCT_CommandType)p_Context->CurrentCommand;
      p_Context->FirstMessage = 0;
   }
   else // memory is not allocated(B_MemAllocResult == 0xff)
   {
      // handle memory allocation error
      dbgmsg("LCT: ERROR memalloc 1",0);
      return PORT_MSG_HNDLR_ERROR;
   }

   if ((Bp_Len == NULL_PTR) || (Bp_msg == NULL_PTR))
   {
      dbgmsg("LCT: ERROR memalloc 2",0);
      return (BYTE)PORT_MSG_HNDLR_ERROR;
   }

   Bp_Reply[0] = LBC_LCT_OK;
   *Bp_Len = 1; //output message length

   //parse command
   switch (B_Command)
   {
      case LCT_Start:
         // set DebugOn mode
         gmvb_DebugOnly = TRUE;
         
         // Version // Protocol version supported by the tool // which version should Athena support // ??
			dbgmsg("version 0x%x", Bp_msg[1]);

         // Version // Highest protocol version supported by the firmware // ??
         Bp_Reply[1] = 0;
         *Bp_Len = 2;
         break;

      case LCT_End:
         {
            dbgmsg("LCT_End",0);
            
            // set DebugOff mode
            gmvb_DebugOnly = FALSE;            
         }
         break;

      case LCT_WriteGammaTable:
         {
            gmt_WriteGammaTable_Req * pReq = (gmt_WriteGammaTable_Req *)Bp_msg;

            BYTE far * pTransferTable= Bp_msg;
            WORD W_Size = GetMesageLength();
            WORD W_Index;
            BYTE * Bp_TempTable = TempBuffer;

				dbgmsg("~ Check W_Size should not over 128 %d", W_Size);

            //Check if it the first entry
            if (p_Context->FirstMessage)
            {
					dbgmsg("TotalSize 0x%x", pReq->TotalSize);
					dbgmsg("VersionNumber 0x%x", pReq->VersionNumber);
					dbgmsg("CompEnable 0x%x", pReq->CompEnable);
					dbgmsg("LengthOfEachColor 0x%x", pReq->LengthOfEachColor); // For Athena is 1024 // ??
					dbgmsg("OffSetOrPtr 0x%x", pReq->OffSetOrPtr); // ??
					dbgmsg("GammaTableoffsetGreen 0x%x", pReq->GammaTableoffsetGreen);
					dbgmsg("GammaTableoffsetBlue 0x%x", pReq->GammaTableoffsetBlue);
					dbgmsg("GammaTableoffsetRed 0x%x", pReq->GammaTableoffsetRed);

					dbgmsg("SteepTableSize 0x%x", pReq->SteepTableSize); // For Athena is 128 // ??
					dbgmsg("SteepStep 0x%x", pReq->SteepStep); // For Athena is 1, 3, 7, or 15.
               // should program to GAMMA_LUT2_CTRL:GAMMA_LUT2_STEEP_STEP

               gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_G, pReq->GreenSteepStart);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_END_G, pReq->GreenSteepEnd);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_G, pReq->GreenSteepFinal);

               gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_B, pReq->BlueSteepStart);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_END_B, pReq->BlueSteepEnd);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_B, pReq->BlueSteepFinal);

               gm_WriteRegWord(GAMMA_LUT2_STEEP_STRT_R, pReq->RedSteepStart);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_END_R, pReq->RedSteepEnd);
               gm_WriteRegWord(GAMMA_LUT2_STEEP_FINAL_R, pReq->RedSteepFinal);

					dbgmsg("SteepTableoffsetGreen 0x%x", pReq->SteepTableoffsetGreen);
					dbgmsg("SteepTableoffsetBlue 0x%x", pReq->SteepTableoffsetBlue);
					dbgmsg("SteepTableoffsetRed 0x%x", pReq->SteepTableoffsetRed);

               pTransferTable += sizeof(gmt_WriteGammaTable_Req);
               W_Size -= sizeof(gmt_WriteGammaTable_Req);
            }

            //cut off data if it is too long for gamma table
            //Cutting off value depends on color component
            {
               WORD W_CutOffValue =sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*3;

               if ((W_Size + sizeof(WORD)*p_Context->Shift+p_Context->StoredFlag) > W_CutOffValue)
               {
                  W_Size = W_CutOffValue -p_Context->Shift;
                  Bp_Reply[0] = LBC_LCT_NotSupported;
						dbgmsg("~ transfer size is over ~", 0);
               }
            }

            //copy input data bufer with taken in mind stored previous data - part of uncompleted gamma table component (low byte of a WORD)
            if (p_Context->StoredFlag)
            {
               Bp_TempTable[0] = p_Context->DataStored;
					dbgmsg("~ Check Point ~ Bp_TempTable[0] 0x%x", Bp_TempTable[0]);
            }

            //take data from input buffer
            memcpy(&Bp_TempTable[p_Context->StoredFlag], (BYTE*)pTransferTable, W_Size);

            //modify Size value if data byte was stored
            W_Size += p_Context->StoredFlag;

            //Load gamatable data.
            //if(p_Context->AllTables) // Always 3 channel
            {
               // if RGB_Component calculate correct shift and color component ID
               if ((W_Size + sizeof(WORD)*p_Context->Shift) >= sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT)
               {
                  //Write the last part of current component data to the VLUT;
                  //Change Color Component ID
                  //Set shift and size for the rest part of recevied table.
                  WORD W_LastPart = sizeof(WORD)*NUMBER_OF_ELEMENTS_IN_GAMMA_LUT -sizeof(WORD)*p_Context->Shift;

                  dbgmsg("W_Size = %d (last part)",W_LastPart);
                  dbgmsg("p_Context->Shift = %d",p_Context->Shift);
                  dbgmsg("p_Context->ColorCmpnt = %d",p_Context->ColorCmpnt);

                  gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_LastPart/2, (WORD far*)Bp_TempTable, 1);

                  if (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
                     p_Context->ColorCmpnt++;

                  p_Context->Shift = 0;
                  W_Size -= W_LastPart;
                  Bp_TempTable += W_LastPart;
               }
            }

            if (W_Size)
            {
               gm_LoadVlutTable(p_Context->ColorCmpnt, p_Context->Shift, W_Size/2, (WORD far*)Bp_TempTable, 1);
            }

            p_Context->Shift += W_Size/sizeof(WORD);

            //store the last byte for next message if data size is odd
            if (W_Size%sizeof(WORD))
            {
               p_Context->StoredFlag= 1;
               p_Context->DataStored = Bp_msg[W_Size-1];
            }
            else
               p_Context->StoredFlag= 0;

            // turn on gamma correction if it is the last message
            if (!MoreDataExpected())
            {
               gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);
            }
         }
         break;

      case LCT_ReadGammaTable:
         {
            WORD W_Index;
            WORD far * Wp_VLUT = (WORD *)TempBuffer;

            //get size for output buffer and make it even
            WORD W_Size = 0xfffe & GetMaxDataLength();

            union
            {
               WORD  w;
               BYTE  b[2];
            }WordVal;

            union
            {
               DWORD  dw;
               BYTE  b[4];
            }DWordVal;

            *Bp_Reply = LBC_LCT_OK;
            *Bp_Len = 1;
            dbgmsg("LCT_ReadGammaTable, shift %d",p_Context->Shift);

            //calculate number of gamma table elements to be send during this transfer
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift) <= (W_Size-2))
            {
               //prepare for the last data transfer message
               W_Size = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift;
            }
            else
            {
               dbgmsg("LCT_ReadGammaTable: More Data",0);
               MoreData(TRUE);
               W_Size -= 2;
            }

            //Check if it the first entry
            if (p_Context->FirstMessage)
            {
               // TotalSize
               DWordVal.dw = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT * 3 + sizeof(gmt_WriteGammaTable_Req) -1;
               *(Bp_Reply++) = DWordVal.b[0];
               *(Bp_Reply++) = DWordVal.b[1];
               *(Bp_Reply++) = DWordVal.b[2];
               *(Bp_Reply++) = DWordVal.b[3];

               // VersionNumber
               *(Bp_Reply++) = TS_LBC_LCT_VER;

               // CompEnable
               *(Bp_Reply++) = 1;

               // LengthOfEachColor
               WordVal.w = NUMBER_OF_ELEMENTS_IN_GAMMA_LUT;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // OffSetOrPtr
               *(Bp_Reply++) = 0;

               // GammaTableoffsetGreen
               *(Bp_Reply++) = 0;

               // GammaTableoffsetBlue
               *(Bp_Reply++) = 0;

               // GammaTableoffsetRed
               *(Bp_Reply++) = 0;

               // Reserved
               *(Bp_Reply++) = 0;
               *(Bp_Reply++) = 0;
               *(Bp_Reply++) = 0;

               // SteepTableSize
               WordVal.w = 128;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // SteepStep
               WordVal.w = (gm_ReadRegWord(GAMMA_LUT2_CTRL) & GAMMA_LUT2_STEEP_ST) >> 8;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // GreenSteepStart
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_STRT_G) & GAMMA_LUT2_STEEP_STRT_GRN;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // GreenSteepEnd
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_END_G) & GAMMA_LUT2_STEEP_END_GRN;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // GreenSteepFinal
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_G) & GAMMA_LUT2_STEEP_FINAL_GRN;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // BlueSteepStart
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_STRT_B) & GAMMA_LUT2_STEEP_STRT_BLU;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // BlueSteepEnd
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_END_B) & GAMMA_LUT2_STEEP_END_BLU;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // BlueSteepFinal
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_B) & GAMMA_LUT2_STEEP_FINAL_BLU;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // RedSteepStart
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_STRT_R) & GAMMA_LUT2_STEEP_STRT_RED;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // RedSteepEnd
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_END_R) & GAMMA_LUT2_STEEP_END_RED;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // RedSteepFinal
               WordVal.w = gm_ReadRegWord(GAMMA_LUT2_STEEP_FINAL_R) & GAMMA_LUT2_STEEP_FINAL_RED;
               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];

               // SteepTableoffsetGreen
               DWordVal.dw = 0;
               *(Bp_Reply++) = DWordVal.b[0];
               *(Bp_Reply++) = DWordVal.b[1];
               *(Bp_Reply++) = DWordVal.b[2];
               *(Bp_Reply++) = DWordVal.b[3];

               // SteepTableoffsetBlue
               DWordVal.dw = 0;
               *(Bp_Reply++) = DWordVal.b[0];
               *(Bp_Reply++) = DWordVal.b[1];
               *(Bp_Reply++) = DWordVal.b[2];
               *(Bp_Reply++) = DWordVal.b[3];

               // SteepTableoffsetRed
               DWordVal.dw = 0;
               *(Bp_Reply++) = DWordVal.b[0];
               *(Bp_Reply++) = DWordVal.b[1];
               *(Bp_Reply++) = DWordVal.b[2];
               *(Bp_Reply++) = DWordVal.b[3];

               W_Size -= sizeof(gmt_WriteGammaTable_Req);
            }

            //read Gamma table block
            {
               WORD W_Shift = p_Context->Shift;

					//gm_Printf("LCT_ReadGammaTable: W_Shift = %d (0x%x)",W_Shift,W_Shift);
					//gm_Printf("LCT_ReadGammaTable: W_Size = %d (0x%x)", W_Size, W_Size);
               {
                  DWORD DW_Addr;

                  //Choose proper LUT addr
                  {
                     DW_Addr =
                        (p_Context->ColorCmpnt == GREEN_VLUT_COMPONENT) ? OUTPUT_GAMMA_R_MEM_START :
                        (p_Context->ColorCmpnt == BLUE_VLUT_COMPONENT) ? OUTPUT_GAMMA_G_MEM_START:
                        OUTPUT_GAMMA_B_MEM_START;
                  }

                  //Read the block of table date to the buffer
                  {
                     WORD W_Index = 0;

                     for (; W_Index < W_Size; W_Index++)
                     {
                        DW_Addr = DW_Addr + W_Shift + W_Index;
                        Wp_VLUT[W_Index] = *LINEAR_TO_FPTR_W(DW_Addr);
                     }
                  }
               }
            }

            for (W_Index = 0 ; W_Index< W_Size ; W_Index+=sizeof(WORD))
            {
               //shift value left to get 16 bits scale.
               WordVal.w = (*Wp_VLUT++)<<(16-GAMMA_TABLE_WIDTH);
               //fill less significant bits by "1"
               WordVal.w |= (0x1 << (16-GAMMA_TABLE_WIDTH))-1;

               *(Bp_Reply++) = WordVal.b[0];
               *(Bp_Reply++) = WordVal.b[1];
            }

            //store counter of transmitted gamma table elements or clear it if gamma table transmitting is completed;
            //prepare context for next message or inform SMT handler that transmitting is completed;
            if ((NUMBER_OF_ELEMENTS_IN_GAMMA_LUT*sizeof(WORD) -p_Context->Shift) == W_Size)
            {
               p_Context->Shift = 0;
               MoreData(FALSE);
               if (p_Context->ColorCmpnt != BLUE_VLUT_COMPONENT)
               {
                  p_Context->ColorCmpnt++;
                  MoreData(TRUE);
                  dbgmsg("LCT_ReadGammaTable: More Data",0);
               }
            }
            else
               p_Context->Shift += W_Size;

            *Bp_Len = W_Size + (p_Context->FirstMessage ? sizeof(gmt_WriteGammaTable_Req):0);
            dbgmsg(" .....done",0);
         }
         break;

      case LCT_WriteZoneUniformityTable:
         {
            union
            {
               DWORD  dw;
               BYTE  b[4];
            }DWordVal;

            DWordVal.b[0] = Bp_msg[1];
            DWordVal.b[1] = Bp_msg[2];
            DWordVal.b[2] = Bp_msg[3];
            DWordVal.b[3] = Bp_msg[4];
				dbgmsg("TableSize %d", DWordVal.dw);

            // UT : TableWidth * TableHeight * NumberOfColors * 2, Uniformity Table data, two bytes per zone per color
            // Program to DUM memory // ??
         }
         break;

      case LCT_ReadZoneUniFormityTable:
         {
            // TableSize 4 bytes
            // UT : TableWidth * TableHeight * NumberOfColors * 2, Uniformity Table data, two bytes per zone per color
         }
         break;

      case LCT_Enable:
         {
            gm_SetRegBitsDWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
         }
         break;

      case LCT_Disable:
         {
            gm_ClearRegBitsDWord(C9VODP_DUM_CONTROL_0, DUM_ENABLE);
         }
         break;

      default:
         // wrong command code received
         Bp_Reply[0] = LBC_LCT_NotSupported;
   }

   //store current command ID in the context.
   p_Context->CurrentCommand = B_Command;

   return PORT_MSG_HNDLR_OK;

}

static PortMsgHndlrReplyType LBC_SMT_ShowBorder(BYTE far * Bp_SMTMsg)
{
   PortMsgHndlrReplyType ErrorCode = PORT_MSG_HNDLR_OK;
   union
   {
      WORD  w;
      BYTE  b[2];
   }WordVal;
   WORD W_OutputWidth = 0;
   WORD W_OutputLength = 0;
   BYTE    SMTBorderWidth, SMTBorderHeight, ModifiedBorderWidth, ModifiedBorderHeight;

   #ifdef ST_4K2K_93xx_BOARD
   if(GetChipID_4K2K()==CHIPID_4K2K_FE)
   {
      W_OutputWidth = gm_ReadRegWord(PB_MAIN_HWIDTH);
      W_OutputLength = gm_ReadRegWord(PB_MAIN_VHEIGHT); 
   }
   else
   #endif      
   {
      W_OutputWidth = PanelWidth;
      W_OutputLength = PanelHeight;   
   }

   dbgmsg("LBC Show Border", 0);

   WordVal.b[0] = Bp_SMTMsg[2];
   WordVal.b[1] = Bp_SMTMsg[3];

   gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_RED, WordVal.w & PB_HLBRD_COLOR_RED); // R

   WordVal.b[0] = Bp_SMTMsg[4];
   WordVal.b[1] = Bp_SMTMsg[5];

   gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_GRN, (WordVal.w >> 5) & PB_HLBRD_COLOR_GRN); // G

   WordVal.b[0] = Bp_SMTMsg[6];
   WordVal.b[1] = Bp_SMTMsg[7];

   gm_ClearAndSetBitsWord(PB_HLBRDR_COLOR, PB_HLBRD_COLOR_BLU, (WordVal.w >> 11) & PB_HLBRD_COLOR_BLU); // B            

   //WordVal.b[0] = Bp_msg[8];
   //WordVal.b[1] = Bp_msg[9];
   // White ?

   // Width
   ModifiedBorderWidth = SMTBorderWidth = Bp_SMTMsg[10];

   #ifdef SMT_LBC_SHOWBORDER_WIDTH_WA
      ModifiedBorderWidth = (BYTE)(((DWORD)((DWORD)SMTBorderWidth * (DWORD)BORDER_WIDTH_SCALE_RATIO * (DWORD)W_OutputWidth)) / ATHENA_R_L_OUTPUT_WIDTH);
      if (SMTBorderWidth > 0 && ModifiedBorderWidth == 0)
         ModifiedBorderWidth = 1;    //minimal 1 pixel
   #endif

   gm_WriteRegWord(PB_HLBRDR_WIDTH, ModifiedBorderWidth);

   #ifdef LBC_SHOWBORDER_BY_R_L      
      if(GetChipID_4K2K()==CHIPID_4K2K_R)
      {
         gm_WriteRegWord(PB_HLBRDR_HACTIVE, W_OutputWidth - (gm_ReadRegWord(PB_HLBRDR_WIDTH)*2));
         gm_WriteRegWord(DH_ACTIVE_START, DH_Active_Start_Saved - gm_ReadRegWord(PB_HLBRDR_WIDTH));
      }
      else if(GetChipID_4K2K()==CHIPID_4K2K_L)
      {
         gm_WriteRegWord(PB_HLBRDR_HACTIVE, W_OutputWidth - (gm_ReadRegWord(PB_HLBRDR_WIDTH)));
      }
   #else // Athena_FE / Standard
      UNUSED_VAR(SMTBorderWidth);
      UNUSED_VAR(SMTBorderHeight);
      gm_WriteRegWord(PB_HLBRDR_HACTIVE, W_OutputWidth - (gm_ReadRegWord(PB_HLBRDR_WIDTH)*2));
   #endif
   
   // Height
   ModifiedBorderHeight = SMTBorderHeight = Bp_SMTMsg[11];
   #ifdef SMT_LBC_SHOWBORDER_WIDTH_WA
   ModifiedBorderHeight= (BYTE)(((DWORD)((DWORD)SMTBorderHeight * (DWORD)BORDER_WIDTH_SCALE_RATIO * (DWORD)W_OutputLength)) / ATHENA_R_L_OUTPUT_HEIGHT);
   if (SMTBorderHeight > 0 && ModifiedBorderHeight == 0)
      ModifiedBorderHeight = 1;   //minimal 1 pixel
   #endif
   gm_WriteRegWord(PB_HLBRDR_HEIGHT, ModifiedBorderHeight);
   gm_WriteRegWord(PB_HLBRDR_VACTIVE, W_OutputLength - (gm_ReadRegWord(PB_HLBRDR_HEIGHT)*2));

   if(Bp_SMTMsg[12])
      gm_SetRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);
   else
      gm_ClearRegBitsWord(PB_CTRL, PB_HL_BORDER_EN);            
    
   //because it is in gmvb_DebugOnly = TRUE after ACT_Start, it has to directly program the host control register to update
   gm_SetRegBitsWord(HOST_CONTROL,ODP_SYNC_UPDATE);

   return ErrorCode;
}
//#endif

void SMT_SendTimeoutTimetoHost(MsgPacket_t *message, BYTE comm_use, WORD Timeout)
{
   BYTE ExpectedEvenOdd;
   BYTE B_TransactionID;
   SMT_TransactionStruct_t SMT_RAM * pCurrentTransaction;

   B_TransactionID = SMT_GetTransID(message);
   pCurrentTransaction = IsTransIDValid(B_TransactionID);
   
   ExpectedEvenOdd = message->msgType & EVEN_ODD_BIT;

   SMT_SendBusy(pCurrentTransaction->ID,comm_use,ExpectedEvenOdd,Timeout);

}

#endif

