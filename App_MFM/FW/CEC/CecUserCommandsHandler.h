#ifndef __CEC_USER_COMMAND_HANDLER_H__
#define __CEC_USER_COMMAND_HANDLER_H__


void CecAppUserCommandHandler(void);

BOOL CecAppUserCommandHandlerRequest(gmt_CecUserCommand *UserCommand);
void CecAppUserCommandHandlerGetResults(gmt_CecUserCommand *CecUserCommand);
gmt_UserCommandState
CecAppUserCommandHandlerGetStatus(void);
BOOL CecAppUserCommandHandlerResetStatus(void);

#endif

