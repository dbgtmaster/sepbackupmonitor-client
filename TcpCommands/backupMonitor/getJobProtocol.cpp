#include "TcpCommands/backupMonitor/getJobProtocol.h"



TcpCommand_backupMonitor_getJobProtocol::TcpCommand_backupMonitor_getJobProtocol(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_getJobProtocol::execWriteData() {
    
    _dataStream << _jobId;
    _dataStream << _protocolType;
    _dataStream << _protocolPage;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_getJobProtocol::execReadData() {

    _dataStream >> _protocol;
    _dataStream >> _maxPages;
}

