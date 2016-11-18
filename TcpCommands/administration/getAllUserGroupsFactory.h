#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPSFACTORY_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_getAllUserGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_getAllUserGroups();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_GETALLUSERGROUPSFACTORY_H

