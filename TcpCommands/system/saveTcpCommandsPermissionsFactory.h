#ifndef TCPCOMMANDFACTORY_SYSTEM_SAVETCPCOMMANDSPERMISSIONSFACTORY_H
#define TCPCOMMANDFACTORY_SYSTEM_SAVETCPCOMMANDSPERMISSIONSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_system_saveTcpCommandsPermissions : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_system_saveTcpCommandsPermissions();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_SYSTEM_SAVETCPCOMMANDSPERMISSIONSFACTORY_H

