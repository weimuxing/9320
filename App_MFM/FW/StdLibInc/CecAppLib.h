#ifndef __VALIDITYTEST_H__
#define __VALIDITYTEST_H__

#include "StdLibInc/gm_Global.h"
#include "StdLibInc/CecMngrLib.h"
//Results of CEC message syntax check
typedef enum
{
   gmd_SCOk                                    = 0x00,
   gmd_SCErrorLen                              = 0x01,
   gmd_SCErrorDir                              = 0x02,
   gmd_SCErrorOpCode                           = 0x04,
   gmd_SCErrorMask                             = 0x07,
   gmd_SCErrorAbort                            = 0x80
}gmt_CecMsgValidityCheckResult;

gmt_CecMsgValidityCheckResult far gm_CecMessageValidityCheck(BYTE B_Code, BYTE B_Length, BOOL Directed);
BYTE far ExtractDigitFromPhysAddr(WORD PhysAddr, BYTE Digit);
BOOL far StatusDidntResponse(gmt_CEC_MNGR_MESSAGE_STATUS fs);

#endif
