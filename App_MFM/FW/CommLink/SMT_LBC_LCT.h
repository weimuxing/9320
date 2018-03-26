


#pragma option -b- // set enum to 8-bit
typedef enum LBC_CommandEnum
{
   LBC_Start=0,
   LBC_End=1,
   LBC_GetCapabilities=2,
   LBC_DisplayGrid=3,
   LBC_DisplayField=4,
   LBC_DisplayZone=5,
   LBC_SetBKColor=6,
   LBC_GetFirmwareRevisionString=7,
   LBC_ShowBorder = 8,
   LBC_SetUnicompMode = 9,
}LBC_CommandType;

typedef enum LCT_CommandEnum
{
   LCT_Start=0,
   LCT_End=1,
   LCT_WriteGammaTable=2,
   LCT_ReadGammaTable=3,
   LCT_WriteZoneUniformityTable=4,
   LCT_ReadZoneUniFormityTable=5,
   LCT_Enable=6,
   LCT_Disable=7
}LCT_CommandType;

typedef enum ACT_CommandEnum
{
   ACT_Start = 0,
   ACT_End = 1,
   ACT_WriteUniformityCompensationTable = 2,
}ACT_CommandType;

typedef enum
{
   LBC_LCT_OK=0,
   LBC_LCT_NotSupported=1,
}LBC_LCTReturnCodeType;
#pragma option -b. // reset to system default


#pragma pack(push)
#pragma pack(1)
typedef struct
{
   WORD Shift:16;
   WORD DataStored:8;
   WORD CurrentCommand:5;
   WORD ColorCmpnt:3;
   WORD AllTables:1;
   WORD CalibrationOn:1;
   WORD StoredFlag:1;
   WORD UnCompletedElement:1;
   WORD ExtraData:1;
   WORD FirstMessage:1;
}LBC_LCTContextType;
#pragma pack(pop)


/**************************************************************************
 Definition for SMT transactions handler to determine context buffersize to be big enough for this prot handler
**************************************************************************/
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE sizeof(GammaContextType)
#elif SMT_MAX_BUF_SIZE < sizeof(GammaContextType)
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE sizeof(GammaContextType)
#endif
/**************************************************************************/



//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE LBC_MsgHandler(MsgPacket_t *message, BYTE comm_use);
BYTE LCT_MsgHandler(MsgPacket_t *message, BYTE comm_use);
BYTE ACT_MsgHandler(MsgPacket_t *message, BYTE comm_use);

