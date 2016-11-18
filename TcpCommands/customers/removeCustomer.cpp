#include "removeCustomer.h"

TcpCommand_customers_removeCustomer::TcpCommand_customers_removeCustomer(QObject *parent) :
    TcpAbstractCommand(parent)
{
}

void TcpCommand_customers_removeCustomer::execWriteData() {
    _dataStream << _id;
}

void TcpCommand_customers_removeCustomer::execReadData() {
}

