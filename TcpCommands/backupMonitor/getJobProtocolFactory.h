#ifndef TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBPROTOCOLFACTORY_H
#define TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBPROTOCOLFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_backupMonitor_getJobProtocol : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_backupMonitor_getJobProtocol();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_BACKUPMONITOR_GETJOBPROTOCOLFACTORY_H

