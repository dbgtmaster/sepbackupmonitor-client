#include "TcpCommands/backupMonitor/getLog.h"

TcpCommand_backupMonitor_getLog::TcpCommand_backupMonitor_getLog(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_getLog::execWriteData() {
    
    _dataStream << _customerID;
    _dataStream << _taskName;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_getLog::execReadData() {
    
    _dataStream >> _rows;
}

