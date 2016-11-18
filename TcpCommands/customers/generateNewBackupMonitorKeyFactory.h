#ifndef TCPCOMMANDFACTORY_CUSTOMERS_GENERATENEWBACKUPMONITORKEYFACTORY_H
#define TCPCOMMANDFACTORY_CUSTOMERS_GENERATENEWBACKUPMONITORKEYFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_generateNewBackupMonitorKey : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_generateNewBackupMonitorKey();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_GENERATENEWBACKUPMONITORKEYFACTORY_H

