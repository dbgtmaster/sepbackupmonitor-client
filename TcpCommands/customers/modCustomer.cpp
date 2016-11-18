#include "TcpCommands/customers/modCustomer.h"

TcpCommand_customers_modCustomer::TcpCommand_customers_modCustomer(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_customers_modCustomer::execWriteData() {

    _dataStream << _customerId;
    _dataStream << _modColumns;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_customers_modCustomer::execReadData() {
    
    _dataStream >> _modStatus;
    _dataStream >> _customerId;
}

