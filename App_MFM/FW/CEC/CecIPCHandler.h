typedef enum
{
   CEC_IPC_DATA,
   CEC_IPC_FRAME_STATUS,
   CEC_IPC_BUS_STATUS,
   CEC_IPC_SET_LOG_ADDR,
   CEC_IPC_GET_TIME,
   CEC_IPC_CONFIG_DATA,
   MAX_CEC_ID
} gmt_CEC_IPC_ID;

void CecIpcHandler(void);
