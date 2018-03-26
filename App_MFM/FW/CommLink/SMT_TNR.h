
#ifdef USE_SMT

#if (FEATURE_TNR == REV3)

#if !defined(SMT_MAX_BUF_SIZE)
#define SMT_MAX_BUF_SIZE 146
#elif SMT_MAX_BUF_SIZE < 146
#undef SMT_MAX_BUF_SIZE
#define SMT_MAX_BUF_SIZE 146
#endif


//******************************************************************************
//  G L O B A L    F U N C T I O N S
//******************************************************************************
BYTE TNR_MsgHandler(MsgPacket_t *message, BYTE comm_use);

#endif // (FEATURE_TNR == REV3)

#endif // USE_SMT

