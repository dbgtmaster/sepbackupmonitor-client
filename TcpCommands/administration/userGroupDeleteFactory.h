#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETEFACTORY_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETEFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_userGroupDelete : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_userGroupDelete();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPDELETEFACTORY_H

