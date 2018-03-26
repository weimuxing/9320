#ifndef __MSG_FWD_H__
#define __MSG_FWD_H__

#define I2C_RD BIT5
#define I2C_ACK BIT3

/* Actual msg length is stripped of the DDC2Bi_CONTROL_STATUS_FLAG*/
#define DDC2Bi_MSG_LENGTH(x) ((x) & (~DDC2Bi_CONTROL_STATUS_FLAG))
//#define CMD_SERIALFWD	0x41	//gprobe command - tunnel through serial port
//#define CMD_I2CFWD		0x42	//gprobe command - tunnel through i2c
#define CMD_FWDMSG		0x41	// gprobe command - generic tunnel

#if 0
typedef enum
{
   COMM_PORT_IDLE,			/* communication port is idle */
   COMM_PORT_WAIT_REPLY	/* communication port is waiting on a reply */
} CommPortStatus_t;

typedef struct
{
   CommPortStatus_t CommPortStatus;
   BYTE ID;
   BYTE WaitID;
} CommPort_t;
#endif

typedef struct COM_CHANNEL
{
   HANDLER_USE_t HandlerType;
   BYTE ID;
} COM_CHANNEL;

#pragma pack(push)
#pragma pack(1)

typedef struct DDC2Bi_VCP_Prefix
{
   BYTE CmdCode;
   WORD CmdPage;
} DDC2Bi_VCP_Prefix;

typedef struct DDC2Bi_Packet
{
   BYTE Dest;
   BYTE Source;
   BYTE Length;
   DDC2Bi_VCP_Prefix VCP;
   BYTE * Message;
   BYTE Checksum;
} DDC2Bi_Packet;

typedef struct FWD_I2C_Packet
{
   BYTE Length;
   BYTE FwdSerialCmd;
   BYTE WhichI2CPort;
   BYTE Message;
   BYTE Checksum;
} FWD_I2C_Packet;


typedef struct FWD_Serial_Packet
{
   BYTE Length;
   BYTE FwdSerialCmd;
   BYTE WhichSerialPort;
   BYTE Message;
   BYTE Checksum;
} FWD_Serial_Packet;

typedef struct FWD_MSG_Header
{
   BYTE Length;					// length of packet
   BYTE FwdCmd;				// gprobe forward command
   BYTE WhichComChannel;		// enum of output communication channel
   BYTE ReadCount;				// number of expected response packets back
   BYTE Delay;					// delay between request-response
   BYTE Timeout;				// timeout before give up on response
} FWD_MSG_Header;


#pragma pack(pop)


extern COM_CHANNEL ComChannel[];

BYTE * DDC2BI_GetResponse(BYTE B_DevId, BYTE * Bp_ResBuf);
void SendSerialPkt(BYTE far *Bp_Buf, BYTE WhichSerialPort);
BYTE far* FwdMsg(BYTE far* Bp_Buf);
BYTE * GetReply(BYTE * Bp_ResponseBuf, HANDLER_USE_t WhichRequestPort, HANDLER_USE_t WhichReplyPort, BYTE TimeOut10ms, BYTE Delay10ms);
void FwdSerialMsg(BYTE far *Bp_Buf, BYTE WhichSerialPort);
void FwdI2CMsg(BYTE far * Bp_Buf, BYTE WhichI2CPort);
BOOL MsgResponseTimedOut(void);
BOOL IsNullMsg(BYTE * Bp_Buf);
void FwdResponse(BYTE far* Bp_Buf, HANDLER_USE_t WhichReplyPort);


#endif //__MSG_FWD__
