#include "TcpCommands/customers/generateNewBackupMonitorKey.h"

TcpCommand_customers_generateNewBackupMonitorKey::TcpCommand_customers_generateNewBackupMonitorKey(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_customers_generateNewBackupMonitorKey::execWriteData() {
    
    _dataStream << _customerId;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_customers_generateNewBackupMonitorKey::execReadData() {
    
    _dataStream >> _newInterfaceKey;
}

