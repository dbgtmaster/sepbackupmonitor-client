#ifndef TCPCOMMANDFACTORY_MAIN_USERLOGIN_H
#define TCPCOMMANDFACTORY_MAIN_USERLOGIN_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_main_userLogin : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_main_userLogin();

protected:
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_MAIN_USERLOGIN_H
