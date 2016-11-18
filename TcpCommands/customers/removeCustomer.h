#ifndef TCPCOMMAND__CUSTOMERS_REMOVECUSTOMER_H
#define TCPCOMMAND__CUSTOMERS_REMOVECUSTOMER_H

#include <TcpAbstractCommand.h>

class TcpCommand_customers_removeCustomer : public TcpAbstractCommand
{
    Q_OBJECT

    quint32 _id;
private:
    
    
public:
    explicit TcpCommand_customers_removeCustomer(QObject *parent = 0);
    
    inline void setCustomerId(quint32 id) { _id = id; }

    void execWriteData();
    void execReadData();
    
public slots:
    
};

#endif // TCPCOMMAND__CUSTOMERS_REMOVECUSTOMER_H

