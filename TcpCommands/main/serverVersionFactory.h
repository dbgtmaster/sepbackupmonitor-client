#ifndef SERVERVERSIONFACTORY_H
#define SERVERVERSIONFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_main_serverVersion : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_main_serverVersion();

    TcpAbstractCommand* createCommand();
};

#endif // SERVERVERSIONFACTORY_H
