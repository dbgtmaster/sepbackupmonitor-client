#include "TcpCommands/backupMonitor/getCustomers.h"

TcpCommand_backupMonitor_getCustomers::TcpCommand_backupMonitor_getCustomers(QObject *parent) : TcpAbstractCommand(parent)
{

}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_getCustomers::execWriteData() {
    
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_getCustomers::execReadData() {
    
    _dataStream >> _customerList;
}

