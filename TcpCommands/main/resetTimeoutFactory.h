#ifndef TCPCOMMANDFACTORY_MAIN_RESETTIMEOUT_H
#define TCPCOMMANDFACTORY_MAIN_RESETTIMEOUT_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_main_resetTimeout : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_main_resetTimeout();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_MAIN_RESETTIMEOUT_H

