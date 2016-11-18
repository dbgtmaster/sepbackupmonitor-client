#include "TcpCommands/backupMonitor/createLog.h"

TcpCommand_backupMonitor_createLog::TcpCommand_backupMonitor_createLog(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_createLog::execWriteData() {
    
    _dataStream << _customerID;
    _dataStream << _taskName;
    _dataStream << _log;

}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_createLog::execReadData() {

}

