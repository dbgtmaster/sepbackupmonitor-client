#ifndef TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDSFACTORY_H
#define TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_system_getAllTcpCommands : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_system_getAllTcpCommands();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMAND_SYSTEM_GETALLTCPCOMMANDSFACTORY_H

