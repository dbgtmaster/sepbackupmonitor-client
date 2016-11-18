#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPSFACTORY_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_userGroupToSystemGroups : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_userGroupToSystemGroups();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_USERGROUPTOSYSTEMGROUPSFACTORY_H

