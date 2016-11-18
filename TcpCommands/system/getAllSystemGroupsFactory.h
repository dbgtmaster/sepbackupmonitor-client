#ifndef TCPCOMMANDFACTORY_SYSTEM_GETALLSYSTEMGROUPSFACTORY_H
#define TCPCOMMANDFACTORY_SYSTEM_GETALLSYSTEMGROUPSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_system_getAllSystemGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_system_getAllSystemGroups();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_SYSTEM_GETALLSYSTEMGROUPSFACTORY_H

