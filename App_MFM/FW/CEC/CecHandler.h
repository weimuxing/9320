#ifndef _CECHANDLER_H_
#define _CECHANDLER_H_

//#define DEVICENAMELENGTH 16
#define STATUSLINELENGTH 20

#define CEC_FRAMES_BUFFER_SIZE  60 //100

typedef struct
{
   WORD PhysicalAddr;
//WORD CecVersion : 5;
//WORD PowerStatus : 1;
   void* DevInfo;
}gmt_CEC_DEVICERECORD;

typedef enum
{
   gmd_ShowOsdStringDefaultTime = 0x0000,
   gmd_ShowOsdString = 0x0001,
   gmd_HideOsdString = 0x0002,
   gmd_NoCommand = 0x0100
}
gmt_OSDSTRINGCOMMAND;

extern gmt_CEC_DEVICERECORD a_CECDevices[];

void CECHandler(void);
void CECInit(void);
WORD GetActiveSourcePhysicalAddress(void);
BYTE GetActiveSourceLogicalAddress(void);
BOOL IsHDMIInput(gmt_UserDefConnNames Port);
WORD InputPortToPhysicalAddress(gmt_UserDefConnNames Port);

void InitPrintToOsd(void);
void ClearPrintOsdBuffer(void);
void PrintToOsd(const char far * str);
void PrintToOsdHandler(void);

#endif
