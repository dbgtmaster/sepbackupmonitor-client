#include "TcpCommands/main/resetTimeoutFactory.h"

#include "TcpCommands/main/resetTimeout.h"

TcpCommandFactory_main_resetTimeout::TcpCommandFactory_main_resetTimeout()
    : TcpAbstractCommandFactory("main", "resetTimeout")
{
    
}

TcpAbstractCommand* TcpCommandFactory_main_resetTimeout::createCommand() {
    
    return new TcpCommand_main_resetTimeout;
}

