#ifndef TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPSFACTORY_H
#define TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_administration_testLDAPGroupMemberships : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_administration_testLDAPGroupMemberships();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_ADMINISTRATION_TESTLDAPGROUPMEMBERSHIPSFACTORY_H

