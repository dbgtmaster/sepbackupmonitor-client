#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPMODFACTORY_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPMODFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_userGroupMod : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_userGroupMod();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPMODFACTORY_H

