#include "TcpCommands/customers/getCustomer.h"

TcpCommand_customers_getCustomer::TcpCommand_customers_getCustomer(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_customers_getCustomer::execWriteData() {
    
    _dataStream << _customerId;
    _dataStream << _columns;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_customers_getCustomer::execReadData() {

    _dataStream >> _row;
}
