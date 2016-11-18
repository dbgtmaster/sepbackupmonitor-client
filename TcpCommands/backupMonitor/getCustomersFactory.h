#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERSFACTORY_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERSFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getCustomers : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getCustomers();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETCUSTOMERSFACTORY_H

