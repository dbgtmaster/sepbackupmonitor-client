#include "TcpCommands/customers/search.h"

TcpCommand_customers_search::TcpCommand_customers_search(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_customers_search::execWriteData() {
    
    _dataStream << _searchFilter;
    _dataStream << _filterColumns;
    _dataStream << _returnColumns;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_customers_search::execReadData() {
    
    _dataStream >> _returnedCustomers;
}

QHash<QString, QVariant> TcpCommand_customers_search::fetchNextCustomer() {

    return _returnedCustomers.takeFirst();
}


bool TcpCommand_customers_search::hasNextCustomer() {

    return (_returnedCustomers.count() != 0);
}
