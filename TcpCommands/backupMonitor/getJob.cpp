#include "TcpCommands/backupMonitor/getJob.h"

TcpCommand_backupMonitor_getJob::TcpCommand_backupMonitor_getJob(QObject *parent) : TcpAbstractCommand(parent)
{
}

// Daten, welche in den Datenstream geschrieben werden...
// Daten werde in "_dataStream << data" geschrieben...
void TcpCommand_backupMonitor_getJob::execWriteData() {
    
    _dataStream << _jobId;
    _dataStream << _getCols;
}

// Daten, welche der Server uns zurÃŒckgesendet hat...
// Daten werden von "_dataStream >> data" gelesen...
void TcpCommand_backupMonitor_getJob::execReadData() {
    
    _dataStream >> _row;
}

