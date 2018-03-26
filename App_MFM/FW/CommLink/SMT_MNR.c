#include <mem.h>
#include "System\All.h"
#include "CommLink\smt.h"
#include "CommLink\SMT_MNR.h"

#ifdef USE_SMT


#define TS_MNR_DEBUG_MSG
#if defined(TS_MNR_DEBUG_MSG)
#define dbgmsg(a,b) gm_Printf(a,b)
#else
#define dbgmsg(a,b)
#endif

#define TS_MNR_VER 0

typedef enum
{
    TS_MNR_START,
    TS_MNR_STOP,
    TS_MNR_READ,
    TS_MNR_WRITE,
    TS_MNR_EN,
    TS_MNR_DISABLE
}TsMNRCmd_t;

extern const DWORD MSNQR_Ctrl3[];
extern VQ_MNR_Parameters_t ROM MNRVQTable;

BYTE SMT_RAM B_MSQNRFilterStrength;

#pragma argsused
BYTE MNR_MsgHandler(MsgPacket_t *message, BYTE comm_use)
{
    BYTE* Bp_msg = (BYTE *)(message->msgpkt);

    //get pointer to output message length setting
    BYTE* Bp_len=GetOutMsgLenPtr();
    BYTE* Bp_out= GetOutMsgBufPtr();

    if (Bp_msg[0] == TS_MNR_START)
    {
        Bp_out[0] = TS_OK;
        Bp_out[1] = TS_MNR_VER;
        *Bp_len = 2;
        dbgmsg("TS_MNR_START %s\n","OK");
        B_MSQNRFilterStrength = MNRVQTable.MSQNRFilterStrength;
    }
    else if (Bp_msg[0] == TS_MNR_STOP)
    {
        Bp_out[0] = TS_OK;
        *Bp_len = 1;
        dbgmsg("TS_MADI_STOP", 0);
    }
    else if (Bp_msg[0] == TS_MNR_READ)
    {
        Bp_out[0] = TS_OK;

        // size
        Bp_out[1] = 13;
        Bp_out[2] = 0;
        Bp_out[3] = 0;
        Bp_out[4] = 0;

        // signature
        Bp_out[5] = 26; // 25;
        Bp_out[6] = 0;
        Bp_out[7] = 0;
        Bp_out[8] = 0;

        // version
        Bp_out[9] = 0;
        Bp_out[10] = 0;
        Bp_out[11] = 0;
        Bp_out[12] = 0;

        // Filter Strength
        Bp_out[13] = B_MSQNRFilterStrength;

        *Bp_len = 14;
        dbgmsg("TS_MNR_READ", 0);

    }
    else if (Bp_msg[0] == TS_MNR_WRITE)
    {
        B_MSQNRFilterStrength = Bp_msg[14];

        // 23:18 GAMMA 17:12 THRESH2 11:6 THRESH1 5:0 THRESH0

        gm_ClearAndSetBitsWord(MPEG_NR_CTRL0, MPEG_NR_GAMMA,(((WORD)(MSNQR_Ctrl3[B_MSQNRFilterStrength] >> 18)&0x3F) << 2));
        gm_WriteRegWord(MPEG_NR_CTRL1, ((WORD)MSNQR_Ctrl3[B_MSQNRFilterStrength]&0xFFF));
        gm_WriteRegWord(MPEG_NR_CTRL2, ((WORD)(MSNQR_Ctrl3[B_MSQNRFilterStrength] >> 12)&0x3F));

        gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);

        Bp_out[0] = TS_OK;
        *Bp_len = 1;

        dbgmsg("TS_MNR_WRITE", 0);
    }
    else if (Bp_msg[0] == TS_MNR_EN)
    {
        Bp_out[0] = TS_OK;
        *Bp_len = 1;
        gm_SetRegBitsWord(MPEG_NR_CTRL0, (MPEG_MOSQ_NR_EN|MPEG_GAUSS_NR_EN));
        gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
        dbgmsg("TS_MNR_EN \n", 0);
    }
    else if (Bp_msg[0] == TS_MNR_DISABLE)
    {
        Bp_out[0] = TS_OK;
        *Bp_len = 1;
        gm_ClearRegBitsWord(MPEG_NR_CTRL0, (MPEG_MOSQ_NR_EN|MPEG_GAUSS_NR_EN));
        gm_SyncUpdate(CH_A, INPUT_OUTPUT_CLOCK);
        dbgmsg("TS_MNR_DISABLE \n", 0);
    }
    else
    {
        dbgmsg("STTS_Handler_MNR: UNKNOWN COMMAND \n", 0);
        Bp_out[0] = TS_NOT_SUPPORTED;
        *Bp_len = 1;
    }


    return (BYTE)PORT_MSG_HNDLR_OK;
}

#endif

