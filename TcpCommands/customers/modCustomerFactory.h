#ifndef TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMERFACTORY_H
#define TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMERFACTORY_H

#include <TcpAbstractCommandFactory.h>

class TcpCommandFactory_customers_modCustomer : public TcpAbstractCommandFactory
{
public:
    TcpCommandFactory_customers_modCustomer();
    
    TcpAbstractCommand* createCommand();
};

#endif // TCPCOMMANDFACTORY_CUSTOMERS_MODCUSTOMERFACTORY_H

