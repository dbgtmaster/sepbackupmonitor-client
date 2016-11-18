#ifndef TCPCOMMANDFACTORY_MAIN_GETUSERCOLUMN_H
#define TCPCOMMANDFACTORY_MAIN_GETUSERCOLUMN_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_loggedInUser_getUserColumn : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_loggedInUser_getUserColumn();

    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_MAIN_GETUSERCOLUMN_H
