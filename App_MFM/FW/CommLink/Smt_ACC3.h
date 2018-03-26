
#ifdef USE_SMT


#if 1
#define ACC3_FULL_SIZE (527 + 2 + 2) // ACC3 Data structure is 527 and 2 for SMT command and 2 for data size counter

#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE ACC3_FULL_SIZE
#elif SMT_MAX_BUF_SIZE < ACC3_FULL_SIZE
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE ACC3_FULL_SIZE
#endif

#else
#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE ACM3_FULL_CONTEXT_SIZE
#elif SMT_MAX_BUF_SIZE < ACM3_FULL_CONTEXT_SIZE
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE ACM3_FULL_CONTEXT_SIZE
#endif
#endif


//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE ACC3_MsgHandler(MsgPacket_t *message, BYTE comm_use);

#endif
